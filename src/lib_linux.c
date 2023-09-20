#include <stdio.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "share.h"
#include "lib_linux.h"

// 返回connect导致阻塞的错误码,在Linux系统下为EINPROGRESS
int l_connect_block_code()
{
    return EINPROGRESS;
}

// 返回send导致阻塞的错误码,在Linux系统下为EAGAIN或EWOULDBLOCK
int l_send_block_code()
{
    return EAGAIN;
}

// 创建epoll,失败则返回-1,成功则返回生成的fd
int l_epoll_create()
{
    // the size parameter of epoll_create() is deprecated
    return epoll_create(1);
}

// 操作epoll事件,失败则返回-1,成功则返回0
int l_epoll_ctl(int epfd, int op, int socket, struct epoll_event *ev)
{
    return epoll_ctl(epfd, op, socket, ev);
}

// 等待epoll事件,失败则返回-1,成功则返回获取事件的数量,可能为0
int l_epoll_wait(int epfd, struct epoll_event *events, int maxEvents, int timeout)
{
    return epoll_wait(epfd, events, maxEvents, timeout);
}

// 返回ip地址字节长度
socklen_t l_address_len()
{
    return INET_ADDRSTRLEN;
}

// 从sockAddr中获取客户端地址字符串,失败则返回-1,成功则返回0
int l_address(struct sockaddr_in *sockAddr, char *addrStr, socklen_t len)
{
    const char *ptr = inet_ntop(AF_INET, &(sockAddr->sin_addr), addrStr, len);
    if (ptr == NULL)
    {
        return -1;
    }
    return 0;
}

// 获取客户端连接的端口号
uint16_t l_port(struct sockaddr_in *sockAddr)
{
    return ntohs(sockAddr->sin_port);
}

// 创建socket,失败则返回-1,成功则返回socket fd
int l_socket_create()
{
    return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

// 接受socket连接,失败则返回-1,成功则返回socket fd,在非阻塞情况下,未立刻建立的连接错误码为EAGAIN或EWOULDBLOCK
int l_accept(int socket, struct sockaddr_in *clientAddr, socklen_t clientAddrSize)
{
    return accept(socket, (struct sockaddr *)clientAddr, &clientAddrSize);
}

// 设置sock地址,失败则返回-1,成功则返回1,地址为非法字符串则返回0
int l_set_sock_addr(struct sockaddr_in *sockAddr, char *address, uint16_t port)
{
    sockAddr->sin_family = AF_INET;
    sockAddr->sin_port = htons(port);
    return inet_pton(AF_INET, address, &(sockAddr->sin_addr));
}

// 设置reuseAddr选项,失败则返回-1,成功则返回0
int l_set_reuse_addr(int socket, int value)
{
    return setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (void *)&value, sizeof(value));
}

// 设置keepalive选项,失败则返回-1,成功则返回0
int l_set_keep_alive(int socket, int value)
{
    return setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (void *)&value, sizeof(value));
}

// 设置tcpnodelay选项,失败则返回-1,成功则返回0
int l_set_tcp_no_delay(int socket, int value)
{
    return setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (void *)&value, sizeof(value));
}

// 获取指定socket上的错误码,如果socket上无错误应返回0
int l_get_err_opt(int socket, int *ptr)
{
    socklen_t ptr_size = sizeof(int);
    return getsockopt(socket, SOL_SOCKET, SO_ERROR, (void *)ptr, &ptr_size);
}

// 设置socket为非阻塞,失败则返回-1,成功则返回0
int l_set_nonblocking(int socket)
{
    int flag = fcntl(socket, F_GETFD, 0);
    return fcntl(socket, F_SETFL, flag | O_NONBLOCK);
}

// bind端口地址,失败则返回-1,成功则返回0
int l_bind(int socket, struct sockaddr_in *sockAddr, socklen_t size)
{
    return bind(socket, (struct sockaddr *)sockAddr, size);
}

// 客户端建立连接,失败则返回-1,成功则返回0
int l_connect(int socket, struct sockaddr_in *sockAddr, socklen_t size)
{
    return connect(socket, (struct sockaddr *)sockAddr, size);
}

// listen端口地址,失败则返回-1,成功则返回0
int l_listen(int socket, int backlog)
{
    return listen(socket, backlog);
}

// 从socket接受数据,失败则返回-1,成功则返回接受的字节数
ssize_t l_recv(int socket, void *buf, size_t len)
{
    return recv(socket, buf, len, 0);
}

// 向socket发送数据,失败则返回-1,成功则返回已接收字节数
ssize_t l_send(int socket, void *buf, size_t len)
{
    return send(socket, buf, len, 0);
}

// 关闭fd,失败则返回-1,成功则返回0
int l_close(int fd)
{
    return close(fd);
}

// 关闭fd写端,失败则返回-1,成功则返回0
int l_shutdown_write(int fd)
{
    return shutdown(fd, SHUT_WR);
}

// 返回当前错误码
int l_errno()
{
    return errno;
}
