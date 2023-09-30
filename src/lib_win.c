#include <errno.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include "wepoll.h"
#include "share.h"
#include "lib_win.h"

#pragma comment(lib, "ws2_32.lib")

int w_connect_block_code()
{
    return WSAEWOULDBLOCK;
}

int w_send_block_code()
{
    return WSAEWOULDBLOCK;
}

int w_interrupt_code()
{
    return WSAEINTR;
}

SOCKET w_invalid_socket()
{
    return INVALID_SOCKET;
}

int w_ipv4_address_len()
{
    return INET_ADDRSTRLEN;
}

int w_ipv6_address_len()
{
    return INET6_ADDRSTRLEN;
}

int w_ipv4_address_size()
{
    return sizeof(struct sockaddr_in);
}

int w_ipv6_address_size()
{
    return sizeof(struct sockaddr_in6);
}

void *w_epoll_create()
{
    return epoll_create(1);
}

int w_epoll_ctl(void *handle, int op, SOCKET socket, struct epoll_event *event)
{
    return epoll_ctl(handle, op, socket, event);
}

int w_epoll_wait(void *handle, struct epoll_event *events, int maxevents, int timeout)
{
    return epoll_wait(handle, events, maxevents, timeout);
}

int w_epoll_close(void *handle)
{
    return epoll_close(handle);
}

int w_get_ipv4_address(struct sockaddr_in *clientAddr, char *addrStr, int len)
{
    const char *result = inet_ntop(AF_INET, &(clientAddr->sin_addr), addrStr, len);
    if (result == NULL)
    {
        return -1;
    }
    return 0;
}

int w_get_ipv6_address(struct sockaddr_in6 *clientAddr, char *addrStr, int len)
{
    const char *result = inet_ntop(AF_INET6, &(clientAddr->sin6_addr), addrStr, len);
    if (result == NULL)
    {
        return -1;
    }
    return 0;
}

u_short w_ipv4_port(struct sockaddr_in *clientAddr)
{
    return ntohs(clientAddr->sin_port);
}

u_short w_ipv6_port(struct sockaddr_in6 *clientAddr)
{
    return ntohs(clientAddr->sin6_port);
}

SOCKET w_ipv4_socket_create()
{
    return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

SOCKET w_ipv6_socket_create()
{
    return socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
}

SOCKET w_accept(SOCKET socket, void *clientAddr, int clientAddrSize)
{
    return accept(socket, (struct sockaddr *)clientAddr, &clientAddrSize);
}

int w_set_ipv4_sock_addr(struct sockaddr_in *sockAddr, char *address, u_short port)
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

int w_set_ipv6_sock_addr(struct sockaddr_in6 *sockAddr, char *address, u_short port)
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

int w_set_reuse_addr(SOCKET socket, int value)
{
    return setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&value, sizeof(value));
}

int w_set_keep_alive(SOCKET socket, int value)
{
    return setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (char *)&value, sizeof(value));
}

int w_set_tcp_no_delay(SOCKET socket, int value)
{
    return setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (char *)&value, sizeof(value));
}

int w_set_ipv6_only(SOCKET socket, int value)
{
    return setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&value, sizeof(value));
}

int w_get_err_opt(SOCKET socket, int *ptr)
{
    int ptr_size = sizeof(int);
    return getsockopt(socket, SOL_SOCKET, SO_ERROR, (char *)ptr, &ptr_size);
}

int w_set_nonblocking(SOCKET socket)
{
    u_long argp = 1;
    return ioctlsocket(socket, FIONBIO, &argp);
}

int w_bind(SOCKET socket, void *sockAddr, int size)
{
    return bind(socket, (SOCKADDR *)sockAddr, size);
}

int w_connect(SOCKET socket, void *sockAddr, int size)
{
    return connect(socket, (SOCKADDR *)sockAddr, size);
}

int w_listen(SOCKET socket, int backlog)
{
    return listen(socket, SOMAXCONN_HINT(backlog));
}

int w_recv(SOCKET socket, char *buf, int len)
{
    return recv(socket, buf, len, 0);
}

int w_send(SOCKET socket, char *buf, int len)
{
    return send(socket, buf, len, 0);
}

int w_close_socket(SOCKET socket)
{
    return closesocket(socket);
}

int w_shutdown_write(SOCKET socket)
{
    return shutdown(socket, SD_SEND);
}

int w_get_last_error()
{
    return WSAGetLastError();
}

int w_clean_up()
{
    return WSACleanup();
}
