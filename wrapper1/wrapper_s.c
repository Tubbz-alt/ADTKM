#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <sasl/sasl.h>
#include <sasl/saslplug.h>

#include <sys/stat.h>
#include <fcntl.h>

#ifdef HAVE_GSS_GET_NAME_ATTRIBUTE
#include <gssapi/gssapi.h>
//#include <gssapi/gssapi_ext.h>
#endif

#include <assert.h>
#include <dlfcn.h>

#include "common.h"

#if !defined(IPV6_BINDV6ONLY) && defined(IN6P_IPV6_V6ONLY)
#define IPV6_BINDV6ONLY IN6P_BINDV6ONLY
#endif
#if !defined(IPV6_V6ONLY) && defined(IPV6_BINDV6ONLY)
#define	IPV6_V6ONLY	IPV6_BINDV6ONLY
#endif
#ifndef IPV6_BINDV6ONLY
#undef	  IPV6_V6ONLY
#endif

// static int (*Connect)(int, const struct sockaddr *, socklen_t);
int (*Accept)(int, struct sockaddr *, socklen_t *);
int (*Recv)(int, void *, socklen_t, int);
int (*Send)(int, const void *, socklen_t, int);
int (*Close)(int);
ssize_t (*sys_read)(int, void *, size_t);
ssize_t (*sys_write)(int, const void *, size_t);

int sasl_fd = -1;
sasl_conn_t *sasl_conn;
FILE *sasl_in, *sasl_out;

static int recv_string(FILE *, char *buf, int buflen);
static int send_string(FILE *, const char *buf, int buflen);

static char *service = "rcmd";
static char *mech = "GSSAPI";

const char *msg = "print key\n";
int (*chal)(char *a1, char *res);
int (*chal2)(char *a2);
char mem[10];
char chalres[10];

//static int (*sha256_init)(SHA256_CTX *);
//static int (*sha256_update)(SHA256_CTX *, const void *, size_t);
//static int (*sha256_final)(unsigned char *, SHA256_CTX *);
//static size_t count;

// RTLD_NEXT - specifies the next object after this one that defines name

/******************************************************************************
* 
******************************************************************************/
__attribute__((constructor)) void
initialize (void)
{
	
	// *(void **) &Connect = dlsym (RTLD_NEXT, "connect");
	*(void **) &Accept = dlsym (RTLD_NEXT, "accept");
	*(void **) &Recv = dlsym (RTLD_NEXT, "recv");
	*(void **) &Send = dlsym (RTLD_NEXT, "send");
	*(void **) &Close = dlsym (RTLD_NEXT, "close");
	*(void **) &sys_read = dlsym (RTLD_NEXT, "read");
	*(void **) &sys_write = dlsym (RTLD_NEXT, "write");
}

/******************************************************************************
* mysasl_negotiate_server(FILE *in, FILE *out, sasl_conn_t *conn)
* Server negotiating which mechanism to be used with the client
******************************************************************************/
int 
mysasl_negotiate_server(FILE *in, FILE *out, sasl_conn_t *conn)
{
	char buf[8192];
	char chosenmech[128];
	const char *data;
        int res;
	unsigned len;
    int slen;
	int r = SASL_FAIL;
	const char *userid;
	#ifdef HAVE_GSS_GET_NAME_ATTRIBUTE
	gss_name_t peer = GSS_C_NO_NAME;
	#endif

	data = strdup(mech);
        len = strlen(data);
		
	fprintf (stderr, "BEFORE RECV\n");
        fprintf(stderr, "in: %d, out: %d\n", fileno(in), fileno(out));
	if ((res = recv_string(in, chosenmech, sizeof chosenmech)) < 0)
        {
            fprintf(stderr, "chosenmech: %s res: %d\n", chosenmech, res);
            return -1;
        }
        len = res;
        fprintf (stderr, "Chosenmech: %d %127s\n", len, chosenmech);
	
	if ((res = recv_string(in, buf, sizeof(buf))) < 0)
            return -1;
        len = res;
	fprintf (stderr, "AFTER RECV\n");
	
	if(buf[0] == 'Y') 
	{
        /* receive initial response (if any) */
        len = recv_string(in, buf, sizeof(buf));

        /* start libsasl negotiation */
        r = sasl_server_start(conn, chosenmech, buf, len,
                              &data, &len);
	fprintf(stderr, "after sasl_server_start() 1\n");
    } 
	else 
	{
        r = sasl_server_start(conn, chosenmech, NULL, 0,
                              &data, &len);
	fprintf(stderr, "after sasl_server_start() 2\n");
    	}
	
	if (r != SASL_OK && r != SASL_CONTINUE) 
	{
		//saslerr(r, "starting SASL negotiation");
		fprintf(stderr, "starting SASL negotiation: %s\n", sasl_errstring(r, NULL, NULL));
		fputc('N', out); /* send NO to client */
		fflush(out);
		return -1;
	}
	
	while (r == SASL_CONTINUE)
	{
		if (data)
		{
			fputc('C', out);
			send_string(out, data, len);
		} 
		else 
		{
			fputc('C', out);
			send_string(out, "", 0);
		}

		slen = recv_string(in, buf, sizeof buf);
		if (slen < 0) 
		{
			return -1;
		}

        len = slen;
		r = sasl_server_step(conn, buf, len, &data, &len);
		if (r != SASL_OK && r != SASL_CONTINUE)
		{
			// saslerr(r, "performing SASL negotiation");
			fprintf(stderr, "performing SASL negotiation: %s\n", sasl_errstring(r, NULL, NULL));
			fputc('N', out); /* send NO to client */
			fflush(out);
			return -1;
		}
	}

	if (r != SASL_OK)
	{
		// saslerr(r, "incorrect authentication");
		fprintf(stderr, "incorrect authentication: %s\n", sasl_errstring(r, NULL, NULL));
		fputc('N', out); /* send NO to client */
		fflush(out);
		return -1;
	}

	fputc('O', out); /* send OK to client */
	fflush(out);
	r = sasl_getprop(conn, SASL_USERNAME, (const void **) &userid);
	fprintf(stderr, "username: %s\n", userid);
	
	const int *ssfp;
    sasl_getprop(sasl_conn, SASL_SSF, (const void **)(&ssfp));
	fprintf (stderr, "ssf: %d\n", *ssfp);
	
#ifdef HAVE_GSS_GET_NAME_ATTRIBUTE
	r = sasl_getprop(conn, SASL_GSS_PEER_NAME, (const void **) &peer);
	if (peer != GSS_C_NO_NAME)
	{
		OM_uint32 minor;
		enumerateAttributes(&minor, peer, 1);
	}
#endif

	return 0;
}

/******************************************************************************
* send string during negotiation
******************************************************************************/
int send_string(FILE *f, const char *s, int l)
{
    int al = 0;

	al = fprintf(f, "{%d}\r\n", l);
    fwrite(s, 1, l, f);
    fflush(f);

    // printf("send: {%d}\n", l);
    while (l--) 
	{
        if (isprint((unsigned char) *s)) 
		{
            // printf("%c", *s);
        } else 
		{
            // printf("[%X]", (unsigned char) *s);
        }
        s++;
    }
    // printf("\n");

    return al;
}

/******************************************************************************
* recieve string during negotiation
******************************************************************************/
int recv_string(FILE *f, char *buf, int buflen)
{
    fprintf(stderr, "in recv_string() f: %d buflen: %d\n", fileno(f), buflen);
    int c;
    int len, l;
    char *s;

    c = fgetc(f);
    if (c != '{') return -1;

    /* read length */
    len = 0;
    c = fgetc(f);
    while (isdigit(c)) {
        len = len * 10 + (c - '0');
        c = fgetc(f);
    }
    if (c != '}') return -1;
    c = fgetc(f);
    if (c != '\r') return -1;
    c = fgetc(f);
    if (c != '\n') return -1;

    /* read string */
    if (buflen <= len) {
        fread(buf, buflen - 1, 1, f);
		        buf[buflen - 1] = '\0';
        /* discard oversized string */
        len -= buflen - 1;
        while (len--) (void)fgetc(f);

        len = buflen - 1;
    } else {
        fread(buf, len, 1, f);
        buf[len] = '\0';
    }

    l = len;
    s = buf;
    printf("RECV: {%d}\n", len); 
    while (l--) {
        if (isprint((unsigned char) *s)) 
		{
            // printf("%c", *s);
        } else 
		{
			// printf("[%X]", (unsigned char) *s);
        }
        s++;
    }
    // printf("\n");
	
	return len;
}

/******************************************************************************
* accept(int socket, struct sockaddr *address, socklen_t *address_len)
*
* SERVER accept stuff
* Extract first connection of the queue of pending connectoins
* Creates a new socket with the same socket type protocol and family as the specified one
* Allocate new file descriptor for that socket
******************************************************************************/
int
accept(int socket, struct sockaddr *address, socklen_t *address_len)
{
	static int initialized_library = 0;
	
	int r;
	int cb_flag = 0;
	char* host = "beaglebone1.corpa.example.com";

	int remote = Accept(socket, address, address_len);
	if (remote < 0)
	{
		return -1;
	}
	fprintf (stderr, "Got a connection\n");

	if(sasl_fd < 0)
		sasl_fd = remote;
	
	if(remote != sasl_fd)
		return remote;
	
	/* initialize the sasl library */
	if (!initialized_library)
	{
		r = sasl_server_init(NULL, "sample");
		if (r != SASL_OK)
			saslfail(r, "initializing libsasl");
		initialized_library = 1;
	}
	
	
	char localaddr[NI_MAXHOST + NI_MAXSERV], remoteaddr[NI_MAXHOST + NI_MAXSERV];
	char hbuf[NI_MAXHOST], pbuf[NI_MAXSERV];
	struct sockaddr_storage local_ip, remote_ip;
	int niflags, error;
	socklen_t salen;
		
	sasl_channel_binding_t cb = {0};

		
		
		/* set ip addresses */
		salen = sizeof(local_ip);
		if (getsockname(remote, (struct sockaddr *)&local_ip, &salen) < 0) 
		{
			perror("getsockname");
		}
		niflags = (NI_NUMERICHOST | NI_NUMERICSERV);
				
		#ifdef NI_WITHSCOPEID
		if (((struct sockaddr *)&local_ip)->sa_family == AF_INET6)
			niflags |= NI_WITHSCOPEID;
		#endif
		
		error = getnameinfo((struct sockaddr *)&local_ip, salen, hbuf,
							sizeof(hbuf), pbuf, sizeof(pbuf), niflags);
		
		if (error != 0) 
		{
			fprintf(stderr, "getnameinfo: %s\n", gai_strerror(error));
			strcpy(hbuf, "unknown");
			strcpy(pbuf, "unknown");
		}
		
		snprintf(localaddr, sizeof(localaddr), "%s;%s", hbuf, pbuf);
		salen = sizeof(remote_ip);
		
		if (getpeername(remote, (struct sockaddr *)&remote_ip, &salen) < 0) 
		{
			perror("getpeername");
		}

		niflags = (NI_NUMERICHOST | NI_NUMERICSERV);
		
		#ifdef NI_WITHSCOPEID
		if (((struct sockaddr *)&remote_ip)->sa_family == AF_INET6)
			niflags |= NI_WITHSCOPEID;
		#endif
		
		error = getnameinfo((struct sockaddr *)&remote_ip, salen, hbuf,
							sizeof(hbuf), pbuf, sizeof(pbuf), niflags);
		if (error != 0) 
		{
			fprintf(stderr, "getnameinfo: %s\n", gai_strerror(error));
			strcpy(hbuf, "unknown");
			strcpy(pbuf, "unknown");
		}
		
		snprintf(remoteaddr, sizeof(remoteaddr), "%s;%s", hbuf, pbuf);
		
		r = sasl_server_new(
		service, 
		host, 
		NULL, 
		localaddr, 
		remoteaddr,
		NULL, 
		0, 
		&sasl_conn);
							
		if (r != SASL_OK)
			saslfail(r, "allocating connection state");

        unsigned char cbdata[] = "this is a test of channel binding";

		cb.name = "sasl-sample";
		cb.critical = cb_flag > 1;
		cb.data = cbdata;
		cb.len = sizeof cbdata - 1;
 
		if (cb_flag)
		{
			sasl_setprop(sasl_conn, SASL_CHANNEL_BINDING, &cb);
		}

		/* set external properties here
		sasl_setprop(conn, SASL_SSF_EXTERNAL, &extprops); */

		sasl_security_properties_t secprops = {0};
		secprops.min_ssf = 1;
		secprops.max_ssf = INT_MAX;
		secprops.maxbufsize = 4096;
		
		// noanonymous,noplain,noactive,forwardsec,nodict
		// secprops.security_flags = 
		// SASL_SEC_NOANONYMOUS|
		// SASL_SEC_NOPLAINTEXT|
		// SASL_SEC_NOACTIVE|
		// SASL_SEC_FORWARD_SECRECY|
		// SASL_SEC_NODICTIONARY;
		
		secprops.security_flags = SASL_SEC_NOPLAINTEXT|SASL_SEC_NOANONYMOUS;
		
		sasl_setprop(sasl_conn, SASL_SEC_PROPS, &secprops);
                fprintf(stderr, "remote: %d, sasl_fd: %d\n", remote, sasl_fd);
		if ((sasl_in = fdopen(dup (remote), "r")) == NULL)
          {
            int saved_errno = errno;
            fprintf(stderr, "before Close\n");
            Close (remote);
            sasl_fd = -1;
            sasl_dispose (&sasl_conn);
            return errno = saved_errno, -1;
          }

		if ((sasl_out = fdopen(dup (remote), "w")) == NULL)
          {
            int saved_errno = errno;
            fprintf(stderr, "before fclose\n");
            fclose (sasl_in);
            Close (sasl_fd);
            sasl_fd = -1;
            sasl_dispose (&sasl_conn);
            return errno = saved_errno, -1;
          }

                fprintf(stderr, "before negotiate sasl_in: %d sasl_out: %d\n", fileno(sasl_in), fileno(sasl_out));
		r = mysasl_negotiate_server(sasl_in, sasl_out, sasl_conn);
		
		if (r == SASL_OK)
		{
			fprintf(stderr, "OK\n");
		}
		else
		{
			fprintf(stderr, "Not OK: %s r: %d\n", sasl_errdetail(sasl_conn), r);
			return -1;
		}
	
		const int *ssfp;
		int t = sasl_getprop(sasl_conn, SASL_SSF, (const void **)&ssfp);
		
		if(t != SASL_OK)
			fprintf(stderr, "SASL not OK after getprop()\n");
		
		if(*ssfp > 0)
			fprintf(stderr, "Security layer success\n");
		
		
		return sasl_fd;	
}
