#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syscall.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>

#include <sasl/sasl.h>
#include <sasl/saslplug.h>

#include <pthread.h>

#include "common.h"

#ifdef HAVE_GSS_GET_NAME_ATTRIBUTE
#include <gssapi/gssapi.h>
//#include <gssapi/gssapi_ext.h>
#endif

#include <assert.h>
#include <dlfcn.h>

int (*Connect)(int, const struct sockaddr *, socklen_t);
int (*Accept)(int, struct sockaddr *, socklen_t *);
int (*Recv)(int, void *, socklen_t, int);
int (*Send)(int, const void *, socklen_t, int);
int (*Close)(int);
ssize_t (*sys_read)(int, void *, size_t);
ssize_t (*sys_write)(int, const void *, size_t);

int sasl_fd = -1;
sasl_conn_t *sasl_conn;
FILE *sasl_in, *sasl_out;

static char *service = "rcmd";
static char *mech = "GSSAPI";

const char *msg = "print key\n";
int (*chal)(char *a1, char *res);
int (*chal2)(char *a2);
char mem[10];
char chalres[10];

/******************************************************************************
* 
******************************************************************************/
__attribute__((constructor)) void
initialize (void)
{
	*(void **) &Connect = dlsym (RTLD_NEXT, "connect");
	*(void **) &Accept = dlsym (RTLD_NEXT, "accept");
	*(void **) &Recv = dlsym (RTLD_NEXT, "recv");
	*(void **) &Send = dlsym (RTLD_NEXT, "send");
	*(void **) &Close = dlsym (RTLD_NEXT, "close");
	*(void **) &sys_read = dlsym (RTLD_NEXT, "read");
	*(void **) &sys_write = dlsym (RTLD_NEXT, "write");
}

/******************************************************************************
* simple(void *context __attribute__((unused)), int id, const char **result, 
	unsigned *len)
* Retrieve authorization ID
******************************************************************************/
static int 
simple(void *context __attribute__((unused)), int id, 
	const char **result, unsigned *len)
{
	static char *bufU = "";
	static char *bufA = "";
	char *b = "";

	fprintf (stderr, "%ld SIMPLE: %d\n", syscall(SYS_gettid), id);
	/* paranoia check */
	if (!result)
	{
		return SASL_BADPARAM;
	}
	
	switch (id)
	{
	case SASL_CB_USER:
		b = bufU;
		break;
	case SASL_CB_AUTHNAME:
		b = bufA;
		break;
	default:
		return SASL_BADPARAM;
	}

	*result = b;
	
	if (len)
		*len = strlen(b);

	fprintf (stderr, "SIMPLE DONE\n");
	return SASL_OK;
}

/******************************************************************************
* callbacks we support
******************************************************************************/
static sasl_callback_t callbacks[] =
{
	{SASL_CB_USER, (sasl_callback_ft)simple, NULL},
	{SASL_CB_AUTHNAME, (sasl_callback_ft)simple, NULL},
	{SASL_CB_LIST_END, NULL, NULL},
};

/******************************************************************************
* send string during negotiation
******************************************************************************/
int send_string(FILE *f, const char *s, int l)
{
	fprintf(f, "{%d}\r\n", l);
    fwrite(s, l, 1, f);
    return fflush(f);
}

/******************************************************************************
* recieve string during negotiation
******************************************************************************/
int recv_string(FILE *f, char *buf, int buflen)
{
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
    // printf("recv: {%d}\n", len); 
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
* mysasl_negotiate_client(FILE *in, FILE *out, sasl_conn_t *conn)
* Client negotiating which mechanism to be used with the server
******************************************************************************/
int 
mysasl_negotiate_client(FILE *in, FILE *out, sasl_conn_t *conn)
{
	fprintf(stderr, "%ld start mysasl_negotiate_client()\n", syscall(SYS_gettid));
	char buf[8192];
	const char *data;
	const char *chosenmech;
	unsigned len;
	int r;
	int c;

	fprintf (stderr, "client_start...\n");
        fflush (stderr);
	r = sasl_client_start(conn, mech, NULL, &data, &len, &chosenmech);
        fprintf (stderr, " done\n");
	
	if (r != SASL_OK && r != SASL_CONTINUE) 
	{
		fprintf(stderr, "saslerror! 1\n");
		return -1;
	}

        fprintf (stderr, "mech: %s\n", chosenmech);
	send_string(out, chosenmech, strlen(chosenmech));
	if(data) 
	{
        send_string(out, "Y", 1);
        send_string(out, data, len);
    }
	else 
	{
        send_string(out, "N", 1);
    }
	
	for (;;) 
	{
		fprintf (stderr, "LOOP\n");
		c = fgetc(in);
		
		switch (c) 
		{
		case 'O':
			goto done_ok;

		case 'N':
			goto done_no;

		case 'C': /* continue authentication */
			break;

		default:
			return -1;
		}
		
		len = recv_string(in, buf, sizeof buf);
		
		r = sasl_client_step(conn, buf, len, NULL, &data, &len);
		if (r != SASL_OK && r != SASL_CONTINUE) 
		{
			fprintf(stderr, "saslerror! %d: %s\n", r, sasl_errstring(r, NULL, NULL));
			return -1;
		}

		if (data) 
		{
			//dprintf(2, "sending response length %d...\n", len);
			send_string(out, data, len);
		}
		else 
		{
			dprintf(2, "sending null response...\n");
			send_string(out, "", 0);
		}
	}

	done_ok:
		fprintf(stderr, "%ld end mysasl() OK\n", syscall(SYS_gettid));
		return 0;

	done_no:
		fprintf(stderr, "%ld end mysasl() NO\n", syscall(SYS_gettid));
		return -1;
}

static int
blocking_connect (int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  int old_mode, r, saved_errno;

  if ((old_mode = set_blocking (sockfd, 1)) < 0)
    {
      return -1;
    }
  fprintf (stderr, "previous blocking mode = %d\n", old_mode);
  r = Connect (sockfd, addr, addrlen);
  saved_errno = errno;
  set_blocking (sockfd, old_mode);
  errno = saved_errno;
  return r;
}


static int initialized = 0;

static void*
lock_new (void)
{
  fprintf(stderr, "%ld lock_new()\n", syscall(SYS_gettid));
  pthread_mutex_t *lockp;
  lockp = calloc (1, sizeof *lockp);
  if (lockp && pthread_mutex_init (lockp, NULL) != 0)
    {
    free (lockp);
    lockp = NULL;
    }
  return lockp;
}

static int
lock_lock (void *lp)
{
  fprintf(stderr, "%ld lock_lock()\n", syscall(SYS_gettid));
  pthread_mutex_t *lockp = lp;
  pthread_mutex_lock (lockp);
  return SASL_OK;
}

static int
lock_unlock (void *lp)
{
  fprintf(stderr, "%ld lock_unlock()\n", syscall(SYS_gettid));
  pthread_mutex_t *lockp = lp;
  pthread_mutex_unlock (lockp);
  return SASL_OK;
}

static void
lock_free (void *lp)
{
  fprintf(stderr, "%ld lock_free()\n", syscall(SYS_gettid));
  pthread_mutex_t *lockp = lp;
  pthread_mutex_destroy (lockp);
  free (lockp);
}

void
initialize_client (void)
{
  fprintf(stderr, "%ld initialize_client()\n", syscall(SYS_gettid));
  sasl_set_mutex (lock_new, lock_lock, lock_unlock, lock_free);
  initialized = sasl_client_init(callbacks);
}

/******************************************************************************
* wconnect(int socket, const struct sockaddr *address, socklen_t address_len)
*
* CLIENT connect stuff
* requests a connection to be made on a socket
******************************************************************************/
static int
connect_(int socket, const struct sockaddr *address, socklen_t address_len)
{
	fprintf(stderr, "%ld start connect_()\n", syscall(SYS_gettid));
	int r;
	char localaddr[NI_MAXHOST + NI_MAXSERV + 1],
           remoteaddr[NI_MAXHOST + NI_MAXSERV + 1];
	char hbuf[NI_MAXHOST], pbuf[NI_MAXSERV];
	unsigned int salen;
	int niflags, error;
	struct sockaddr_storage local_ip, remote_ip;
	int cb_flag = 0;
	const char* host = "beaglebone2.corpa.example.com";
	sasl_channel_binding_t cb = {0};

	if (address->sa_family != AF_INET)
          {
          return Connect (socket, address, address_len);
          }

	r = blocking_connect(socket, address, address_len);
	if (r < 0)
	{
		int saved_errno = errno;
		perror ("connect");
		errno = saved_errno;
		return r;
	}
	
	if(sasl_fd < 0)
		sasl_fd = socket;
		
	if(socket != sasl_fd)
		return r;

	/* set ip addresses */
	salen = sizeof(local_ip);
	if (getsockname(socket, (struct sockaddr *)&local_ip, &salen) < 0)
	{
		perror("getsockname");
	}

	niflags = (NI_NUMERICHOST | NI_NUMERICSERV);

	#ifdef NI_WITHSCOPEID
	if (local_ip.ss_family == AF_INET6)
	{
		niflags |= NI_WITHSCOPEID;
	}
	#endif

	error = getnameinfo((struct sockaddr *)&local_ip, salen,
						hbuf, sizeof(hbuf), pbuf, sizeof(pbuf), niflags);

	if (error != 0)
	{
		fprintf(stderr, "getnameinfo: %s\n", gai_strerror(error));
		strcpy(hbuf, "unknown");
		strcpy(pbuf, "unknown");
	}

	snprintf(localaddr, sizeof(localaddr), "%s;%s", hbuf, pbuf);
	salen = sizeof(remote_ip);

	if (getpeername(sasl_fd, (struct sockaddr *)&remote_ip, &salen) < 0)
	{
		perror("getpeername");
	}

	niflags = (NI_NUMERICHOST | NI_NUMERICSERV);

	#ifdef NI_WITHSCOPEID
	if (remote_ip.ss_family == AF_INET6)
		niflags |= NI_WITHSCOPEID;
	#endif

	error = getnameinfo((struct sockaddr *)&remote_ip, salen,
						hbuf, sizeof(hbuf), pbuf, sizeof(pbuf), niflags);

	if (error != 0)
	{
		fprintf(stderr, "getnameinfo: %s\n", gai_strerror(error));
		strcpy(hbuf, "unknown");
		strcpy(pbuf, "unknown");
	}

	snprintf(remoteaddr, sizeof(remoteaddr), "%s;%s", hbuf, pbuf);

	/* client new connection */
	r = sasl_client_new(service, host, NULL, NULL, NULL, 0, &sasl_conn);

	if (r != SASL_OK)
		saslfail(r, "allocating connection state");

	if (cb_flag)
	{
		cb.name = "sasl-sample";
		cb.critical = cb_flag > 1;
		const char * s = "this is a test of channel binding";
		const unsigned char * temp = (const unsigned char *)s;
		cb.data = temp;
		cb.len = (unsigned long)(strlen((char*)cb.data));

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
	
	if ((sasl_in = fdopen (dup (sasl_fd), "r")) == NULL)
      {
        int saved_errno = errno;
        Close (sasl_fd); sasl_fd = -1;
        sasl_dispose (&sasl_conn);
        return errno = saved_errno, -1;
      }

    if ((sasl_out = fdopen (dup (sasl_fd), "w")) == NULL)
      {
        int saved_errno = errno;
        fclose (sasl_in);
        Close (sasl_fd); sasl_fd = -1;
        sasl_dispose (&sasl_conn);
        return errno = saved_errno, -1;
      }

	fprintf (stderr, "BEFORE\n");
	r = mysasl_negotiate_client(sasl_in, sasl_out, sasl_conn);
	fprintf (stderr, "AFTER\n");

	const int *ssfp;
	int t = sasl_getprop(sasl_conn, SASL_SSF, (const void**)&ssfp);
	
	if(t != SASL_OK)
			fprintf(stderr, "SASL not OK after getprop()\n");
	
	if(*ssfp > 0)
		fprintf(stderr, "Security layer success\n");
        fclose (sasl_out);
        fclose (sasl_in);
	if (r == SASL_OK)
	{
		fprintf(stderr, "end connect_() OK\n");
		return 0;
	}
	else
	{
		fprintf(stderr, "end connect_() Not OK: %s\n", sasl_errstring(r, NULL, NULL));
		return -1;
	}
}

int
connect(int socket, const struct sockaddr *address, socklen_t address_len)
{
  //inet_ntop(AF_INET, &(address).sin_addr, str, INET_ADDRSTRLEN);
  struct sockaddr_in *sinT = (struct sockaddr_in*) address;
  
  static int kinitStarted = 0;
  
  fprintf(stderr, "%ld start connect() on %s\n", syscall(SYS_gettid), inet_ntoa (sinT->sin_addr));
  static pthread_once_t once = PTHREAD_ONCE_INIT;
  //static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  int r, old_mode;
  
  // if locked, return
  // if not, continue
  

  fprintf(stderr, "%ld pthread_once\n", syscall(SYS_gettid));
  pthread_once (&once, initialize_client);
  //fprintf(stderr, "%ld pthread_mutex_lock\n", syscall(SYS_gettid));
  
  //pthread_mutex_lock (&lock);
  if(kinitStarted == 0)
  {
    kinitStarted = 1;
  
  fprintf (stderr, "%ld IN CONNECT\n", syscall(SYS_gettid));
  if (address->sa_family == AF_INET)
    {
    struct sockaddr_in *sin = (struct sockaddr_in*) address;
    fprintf (stderr, "Socket: %d, Who %s, Port: %d\n", socket, inet_ntoa (sin->sin_addr), ntohs (sin->sin_port));
    }
  if ((old_mode = set_blocking (socket, 1)) < 0)
    {
      fprintf(stderr, "end connect()\n");
      return -1;
    }

  
  r = connect_ (socket, address, address_len);
  set_blocking (socket, old_mode);
  }
  else
  {
    return Connect (socket, address, address_len);
  }
  //pthread_mutex_unlock (&lock);
  fprintf(stderr, "end connect()\n");
  return r;
}
