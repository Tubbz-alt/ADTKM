#ifndef INCLUDED_COMMON_H
#define INCLUDED_COMMON_H

#include <sys/types.h>
#include <sys/socket.h>

extern int sasl_fd;
extern int (*Close)(int);
extern int (*Recv)(int, void *, socklen_t, int);
extern int (*Send)(int, const void *, socklen_t, int);

_Noreturn void saslfail (int saslerr, const char *format, ...);
int set_blocking (int sockfd, int blocking);
int send_packet (int sockfd, const void *packet, size_t len, int flags);


#endif /* !INCLUDED_COMMON_H */
