#include "common.h"

#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

#include <sasl/sasl.h>      
#include <sasl/saslplug.h>     

extern int sasl_fd;
extern int (*Close)(int);
extern int (*Recv)(int, void *, socklen_t, int);
extern int (*Send)(int, const void *, socklen_t, int);
extern ssize_t (*sys_read)(int, void *, size_t);
extern ssize_t (*sys_write)(int, const void *, size_t);
extern sasl_conn_t *sasl_conn;
extern FILE *sasl_in, *sasl_out;

static char recvbuf[16 * 1024];
static size_t nrecvbuf = 0;

static int
writeall_ret(int fd, const void *buf, size_t buflen)
{
  const char *cp = buf;
  size_t todo = buflen;
  int saved_errno = errno;
  ssize_t nr;

  while (todo > 0)
  {
    if ((nr = write(fd, cp, todo)) < 0)
    {
      saved_errno = errno;
      if (saved_errno == EINTR)
      {
        continue;
      }
      perror("writeall");
      break;
    }
    cp += nr, todo -= nr;
  }
  errno = saved_errno;
  return todo > 0 ? -1 : 0;
}

_Noreturn void
saslfail(int saslerr, const char *format, ...)
{
  char f[1024], msg[1024];
  va_list arglist;
  int fd = fileno(stderr);
  size_t len;

  va_start(arglist, format);
  snprintf(f, sizeof f, "%s: SASLERR(%d): %.100s",
           format, saslerr, sasl_errstring(saslerr, NULL, NULL));
  vsnprintf(msg, sizeof msg, f, arglist);
  len = strlen(msg);
  /* msg may not be null terminated! */
  msg[len++] = '\n';
  writeall_ret(fd, msg, len);
  exit(255);
}

static int
wait_for_write_ready(int fd)
{
  struct pollfd event = {.fd = fd, .events = POLLOUT, .revents = 0};
  int npoll;

  do
  {
    npoll = poll(&event, 1, -1);
  } while (npoll < 0 && errno == EINTR);

  return npoll == 1;
}

int set_blocking(int fd, int blocking)
{
  int flags;
  if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
  {
    return -1;
  }
  if (fcntl(fd, F_SETFL, blocking ? flags & ~O_NONBLOCK : flags | O_NONBLOCK) == -1)
  {
    return -1;
  }
  /* return old blocking mode */
  return (flags & O_NONBLOCK) != O_NONBLOCK;
}

int send_packet(int fd, const void *packet, size_t packetlen, int flags)
{
  const char *cp = packet;
  size_t todo = packetlen;
  ssize_t nr;
  int saved_errno;

  fprintf(stderr, "send_packet sending %zu bytes\n", packetlen);
  if (packetlen > 0)
  {
    fprintf(stderr, "sending:");
    for (size_t i = 0; i < todo; i++)
    {
      fprintf(stderr, " %02x", cp[i]);
    }
    fprintf(stderr, "\n");
  }

  if ((nr = Send(fd, packet, packetlen, flags)) < 0)
  {
    return -1;
  }

  /* made some progress, now it's imperative I complete */
  saved_errno = errno;
  cp += nr, todo -= nr;
  while (todo > 0)
  {
    if ((nr = Send(fd, cp, todo, flags)) < 0)
    {
      saved_errno = errno;
      if (saved_errno == EINTR)
      {
        continue;
      }
      if (saved_errno == EAGAIN)
      {
        wait_for_write_ready(fd);
        continue;
      }
      perror("send_packet");
      break;
    }
  }
  errno = saved_errno;
  return todo > 0 ? -1 : 0;
}

ssize_t
send(int socket, const void *message, size_t length, int flags)
{
  const char *input = message;
  const char *output;
  unsigned outputlen;
  int result;

  if (socket != sasl_fd)
  {
    fprintf(stderr, "send - socket != sasl_fd\n");
    return Send(socket, input, length, flags);
  }
  fprintf(stderr, "sending %zu bytes\n", length);
  result = sasl_encode(sasl_conn,
                       input, length,
                       &output, &outputlen);

  if (result != SASL_OK)
  {
    fprintf(stderr, "bad sasl: %s\n", sasl_errstring(result, NULL, NULL));
    return errno = EIO, -1;
  }

  if ((result = send_packet(socket, output, outputlen, flags)) < 0)
  {
    return -1;
  }

  return length;
}

ssize_t
recv(int socket, void *buffer, size_t length, int flags)
{
  char net[8192];
  const char *tempBuffer = NULL;
  unsigned tempLength = 0;
  int saved_errno;

  ssize_t net_in;
  ssize_t r;

  if (socket != sasl_fd)
  {
    fprintf(stderr, "recv - socket != sasl_fd\n");
    return Recv(socket, buffer, length, flags);
  }

  fprintf(stderr, "in recv: %zu flags: %d\n", length, flags);

  if (nrecvbuf > 0)
  {
    int old_mode;

    old_mode = set_blocking(sasl_fd, 0);
    net_in = Recv(sasl_fd, net, sizeof net, flags);
    saved_errno = errno;
    set_blocking(sasl_fd, old_mode);
    errno = saved_errno;

    // if old_mode == blocking and errno == EAGAIN
    // that's us
    if (net_in <= 0)
    {
      goto copy_out;
    }
  }
  else
  {
    net_in = Recv(sasl_fd, net, sizeof net, flags);
  }

  saved_errno = errno;
  fprintf(stderr, "rec: %zd\n", net_in);
  if (net_in <= 0)
  {
    if (net_in < 0)
    {
      perror("recv");
    }
    return errno = saved_errno, net_in;
  }

  uint32_t sum = 0;
  for (size_t i = 0; i < (size_t)net_in; i++)
  {
    sum += (uint8_t)net[i];
  }
  fprintf(stderr, "recv sum %lu\n", (long unsigned)sum);
  fprintf(stderr, "net recv: %zd\n", net_in);

  int result = sasl_decode(sasl_conn,
                           net, net_in,
                           &tempBuffer, &tempLength);

  for (size_t j = 0; j < tempLength; j++)
  {
    fprintf(stderr, "%02x ", (unsigned char)tempBuffer[j]);
  }
  fprintf(stderr, "\n");
  if (result != SASL_OK)
  {
    fprintf(stderr, "recv - bad decode: %s\n", sasl_errstring(result, NULL, NULL));
    errno = EIO;
    r = -1;
    goto done;
  }

  if (sizeof recvbuf - nrecvbuf < tempLength) // buffer overflow 
  {
    printf("ABORT HIT\n");
    abort();
  }

  /* concatenate data to end of buffer */
  memcpy(&recvbuf[nrecvbuf], tempBuffer, tempLength);
  nrecvbuf += tempLength;

/* determine how much to respond with */
copy_out:
  r = length < nrecvbuf ? length : nrecvbuf;
  /* copy to user buffer */
  memcpy(buffer, recvbuf, r);
  /* move data down */  
  memmove(&recvbuf[0], &recvbuf[r], nrecvbuf -= r);
done:
  fprintf(stderr, "recv returning %zd\n", r);
  if (r > 0)
  {
    const unsigned char *cp = buffer;
    fprintf(stderr, "user buffer: %02x", cp[0]);
    for (size_t i = 1, n = r; i < n; i++)
    {
      fprintf(stderr, " %02x", cp[i]);
    }
    fprintf(stderr, "\n");
  }
  return r;
}

int close(int socket)
{
  int r = Close(socket);
  int saved_errno = errno;
  if (socket == sasl_fd)
  {
    fprintf(stderr, "sasl is shutting down\n");
    fclose(sasl_in), sasl_in = NULL;
    fclose(sasl_out), sasl_out = NULL;
    sasl_dispose(&sasl_conn), sasl_conn = NULL;
    sasl_fd = -1;
    nrecvbuf = 0;
  }
  errno = saved_errno;
  return r;
}

ssize_t
read(int fd, void *buf, size_t n)
{
  if (fd != sasl_fd)
  {
    return sys_read(fd, buf, n);
  }
  return recv(fd, buf, n, 0);
}

ssize_t
write(int fd, const void *buf, size_t n)
{
  if (fd != sasl_fd)
  {
    return sys_write(fd, buf, n);
  }
  return send(fd, buf, n, 0);
}
