#include <stdio.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "share.h"
#include "lib_macos.h"

int m_connect_block_code()
{
    return EINPROGRESS;
}

int m_send_block_code()
{
    return EAGAIN;
}

int m_interrupt_code()
{
    return EINTR;
}

int m_ipv4_address_len()
{
    return INET_ADDRSTRLEN;
}

int m_ipv6_address_len()
{
    return INET6_ADDRSTRLEN;
}

int m_kqueue()
{
    return kqueue();
}

int m_kevent_ctl(int kq, int fd, int16_t filter, uint16_t flags)
{
    struct kevent event;
    EV_SET(&event, fd, filter, flags, 0, 0, NULL);
    return kevent(kq, &event, 1, NULL, 0, NULL);
}

int m_kevent_wait(int kq, struct kevent *eventlist, int nevents, struct timespec *timeout)
{
    return kevent(kq, NULL, 0, eventlist, nevents, timeout);
}

int m_ipv4_address_size()
{
    return sizeof(struct sockaddr_in);
}

int m_ipv6_address_size()
{
    return sizeof(struct sockaddr_in6);
}

int m_get_ipv4_address(struct sockaddr_in *sockAddr, char *addrStr, socklen_t len)
{
    const char *result = inet_ntop(AF_INET, &(sockAddr->sin_addr), addrStr, len);
    if (result == NULL)
    {
        return -1;
    }
    return 0;
}

int m_get_ipv6_address(struct sockaddr_in6 *sockAddr, char *addrStr, socklen_t len)
{
    const char *result = inet_ntop(AF_INET6, &(sockAddr->sin6_addr), addrStr, len);
    if (result == NULL)
    {
        return -1;
    }
    return 0;
}

uint16_t m_ipv4_port(struct sockaddr_in *sockAddr)
{
    return ntohs(sockAddr->sin_port);
}

uint16_t m_ipv6_port(struct sockaddr_in6 *sockAddr)
{
    return ntohs(sockAddr->sin6_port);
}

int m_ipv4_socket_create()
{
    return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

int m_ipv6_socket_create()
{
    return socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
}

int m_accept(int socket, void *clientAddr, socklen_t clientAddrSize)
{
    return accept(socket, (struct sockaddr *)clientAddr, &clientAddrSize);
}

int m_set_ipv4_sock_addr(struct sockaddr_in *sockAddr, char *address, uint16_t port)
{
    sockAddr->sin_family = AF_INET;
    sockAddr->sin_port = htons(port);
    if (address == NULL)
    {
        sockAddr->sin_addr.s_addr = INADDR_ANY;
        return 1;
    }
    else
    {
        return inet_pton(AF_INET, address, &(sockAddr->sin_addr));
    }
}

int m_set_ipv6_sock_addr(struct sockaddr_in6 *sockAddr, char *address, uint16_t port)
{
    sockAddr->sin6_family = AF_INET6;
    sockAddr->sin6_port = htons(port);
    if (address == NULL)
    {
        sockAddr->sin6_addr = in6addr_any;
        return 1;
    }
    else
    {
        return inet_pton(AF_INET6, address, &(sockAddr->sin6_addr));
    }
}

int m_set_reuse_addr(int socket, int value)
{
    return setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (void *)&value, sizeof(value));
}

int m_set_keep_alive(int socket, int value)
{
    return setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (void *)&value, sizeof(value));
}

int m_set_tcp_no_delay(int socket, int value)
{
    return setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (void *)&value, sizeof(value));
}

int m_set_ipv6_only(int socket, int value)
{
    return setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (void *)&value, sizeof(value));
}

int m_get_err_opt(int socket, int *ptr)
{
    socklen_t ptr_size = sizeof(int);
    return getsockopt(socket, SOL_SOCKET, SO_ERROR, (void *)ptr, &ptr_size);
}

int m_set_nonblocking(int socket)
{
    int flag = fcntl(socket, F_GETFD, 0);
    return fcntl(socket, F_SETFL, flag | O_NONBLOCK);
}

int m_bind(int socket, void *sockAddr, int size)
{
    return bind(socket, (struct sockaddr *)sockAddr, size);
}

int m_connect(int socket, void *sockAddr, int size)
{
    return connect(socket, (struct sockaddr *)sockAddr, size);
}

int m_listen(int socket, int backlog)
{
    return listen(socket, backlog < SOMAXCONN ? backlog : SOMAXCONN);
}

int m_recv(int socket, void *buf, int len)
{
    return recv(socket, buf, len, 0);
}

int m_send(int socket, void *buf, int len)
{
    return send(socket, buf, len, 0);
}

int m_close(int fd)
{
    return close(fd);
}

int m_shutdown_write(int fd)
{
    return shutdown(fd, SHUT_WR);
}

int m_errno()
{
    return errno;
}
