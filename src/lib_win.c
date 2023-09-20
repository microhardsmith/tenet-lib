#include <errno.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include "wepoll.h"
#include "share.h"
#include "lib_win.h"

#pragma comment(lib, "ws2_32.lib")

// 获取标准输入流
FILE *g_stdout()
{
    return stdout;
}

// 获取标准错误流
FILE *g_stderr()
{
    return stderr;
}

// 输出字符串至指定流
void g_print(char *str, FILE *stream)
{
    fputs(str, stream);
    fflush(stream);
}

// 返回connect导致阻塞的错误码
int w_connect_block_code()
{
    return WSAEWOULDBLOCK;
}

// 返回send导致阻塞的错误码
int w_send_block_code()
{
    return WSAEWOULDBLOCK;
}

// 返回无效socket值
SOCKET w_invalid_socket()
{
    return INVALID_SOCKET;
}

// 获取IP地址字符串长度
int w_address_len()
{
    return INET_ADDRSTRLEN;
}

// 创建epoll,返回创建的epoll句柄
void *w_epoll_create()
{
    // the size parameter means noting now, must be positive
    return epoll_create(1);
}

// epoll_ctl 操控事件,失败则返回-1,成功则返回0
int w_epoll_ctl(void *handle, int op, SOCKET socket, struct epoll_event *event)
{
    return epoll_ctl(handle, op, socket, event);
}

// epoll_wait,出现错误则返回-1,否则返回触发的事件个数
int w_epoll_wait(void *handle, struct epoll_event *events, int maxevents, int timeout)
{
    return epoll_wait(handle, events, maxevents, timeout);
}

// epoll_close,出现错误则返回-1,否则返回0
int w_epoll_close(void *handle)
{
    return epoll_close(handle);
}

// 获取客户端连接的地址,写入到指定addrStr下出现错误则返回-1,否则返回0
int w_address(struct sockaddr_in *clientAddr, char *addrStr, int len)
{
    const char *result = inet_ntop(AF_INET, &(clientAddr->sin_addr), addrStr, len);
    if (result == NULL)
    {
        return -1;
    }
    return 0;
}

// 获取客户端连接的端口号
u_short w_port(struct sockaddr_in *clientAddr)
{
    return ntohs(clientAddr->sin_port);
}

// 创建socket,失败则返回-1,成功则返回socket值
SOCKET w_socket_create()
{
    return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

// accept 连接,出现错误则返回-1,否则返回建立的fd
SOCKET w_accept(SOCKET socket, struct sockaddr_in *clientAddr, int clientAddrSize)
{
    return accept(socket, (struct sockaddr *)clientAddr, &clientAddrSize);
}

// 设置sock addr,失败则返回-1,如果address字符串不合法会返回0,否则返回1
int w_set_sock_addr(struct sockaddr_in *sockAddr, char *address, u_short port)
{
    sockAddr->sin_family = AF_INET;                                                           // 使用IPv4地址
    sockAddr->sin_port = htons(port);                                                         // 设置端口
    return inet_pton(AF_INET, address == NULL ? INADDR_ANY : address, &(sockAddr->sin_addr)); // 设置IP地址
}

// 设置TCP_NODELAY为指定值,失败返回-1,成功返回0
int w_set_reuse_addr(SOCKET socket, int value)
{
    return setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&value, sizeof(value));
}

// 设置TCP_NODELAY为指定值,失败返回-1,成功返回0
int w_set_keep_alive(SOCKET socket, int value)
{
    return setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (char *)&value, sizeof(value));
}

// 设置TCP_NODELAY为指定值,失败返回-1,成功返回0
int w_set_tcp_no_delay(SOCKET socket, int value)
{
    return setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (char *)&value, sizeof(value));
}

// 获取指定socket上的错误码,如果socket上无错误应返回0
int w_get_err_opt(SOCKET socket, int *ptr)
{
    int ptr_size = sizeof(int);
    return getsockopt(socket, SOL_SOCKET, SO_ERROR, (char *)ptr, &ptr_size);
}

// 设置指定socket为非阻塞,失败返回-1,成功返回0
int w_set_nonblocking(SOCKET socket)
{
    u_long argp = 1;
    return ioctlsocket(socket, FIONBIO, &argp);
}

// 绑定socket到固定端口,失败则返回-1,成功则返回0
int w_bind(SOCKET socket, struct sockaddr_in *sockAddr, int size)
{
    return bind(socket, (SOCKADDR *)sockAddr, size);
}

// 客户端建立连接,失败则返回-1,成功则返回0
int w_connect(SOCKET socket, struct sockaddr_in *sockAddr, int size)
{
    return connect(socket, (SOCKADDR *)sockAddr, size);
}

// 设置socket监听,失败则返回-1,成功则返回0
int w_listen(SOCKET socket, int backlog)
{
    return listen(socket, SOMAXCONN_HINT(backlog));
}

// 从socket接受数据,失败则返回-1,成功则返回已接收字节数,远端已关闭则返回0
int w_recv(SOCKET socket, char *buf, int len)
{
    return recv(socket, buf, len, 0);
}

// 向socket发送数据,失败则返回-1,成功则返回已接收字节数
int w_send(SOCKET socket, char *buf, int len)
{
    return send(socket, buf, len, 0);
}

// 关闭socket连接,失败则返回-1,成功则返回0
int w_close_socket(SOCKET socket)
{
    return closesocket(socket);
}

// 关闭socket写端,失败则返回-1,成功则返回0
int w_shutdown_write(SOCKET socket)
{
    return shutdown(socket, SD_SEND);
}

// 获取上一个WSA错误码
int w_get_last_error()
{
    return WSAGetLastError();
}

// 清理当前wsa使用,失败则返回-1,成功则返回0
int w_clean_up()
{
    return WSACleanup();
}
