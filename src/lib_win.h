#ifndef TENET_WIN_H
#define TENET_WIN_H

EXPORT_SYMBOL FILE *g_stdout();

EXPORT_SYMBOL FILE *g_stderr();

EXPORT_SYMBOL void g_print(char *str, FILE *stream);

EXPORT_SYMBOL int w_connect_block_code();

EXPORT_SYMBOL int w_send_block_code();

EXPORT_SYMBOL SOCKET w_invalid_socket();

EXPORT_SYMBOL int w_address_len();

EXPORT_SYMBOL void *w_epoll_create();

EXPORT_SYMBOL int w_epoll_ctl(void *handle, int op, SOCKET socket, struct epoll_event *event);

EXPORT_SYMBOL int w_epoll_wait(void *handle, struct epoll_event *events, int maxevents, int timeout);

EXPORT_SYMBOL int w_epoll_close(void *handle);

EXPORT_SYMBOL int w_address(struct sockaddr_in *clientAddr, char *addrStr, int len);

EXPORT_SYMBOL u_short w_port(struct sockaddr_in *clientAddr);

EXPORT_SYMBOL SOCKET w_socket_create();

EXPORT_SYMBOL SOCKET w_accept(SOCKET socket, struct sockaddr_in *clientAddr, int clientAddrSize);

EXPORT_SYMBOL int w_set_sock_addr(struct sockaddr_in *sockAddr, char *address, u_short port);

EXPORT_SYMBOL int w_set_reuse_addr(SOCKET socket, int value);

EXPORT_SYMBOL int w_set_keep_alive(SOCKET socket, int value);

EXPORT_SYMBOL int w_set_tcp_no_delay(SOCKET socket, int value);

EXPORT_SYMBOL int w_get_err_opt(SOCKET socket, int *ptr);

EXPORT_SYMBOL int w_set_nonblocking(SOCKET socket);

EXPORT_SYMBOL int w_bind(SOCKET socket, struct sockaddr_in *sockAddr, int size);

EXPORT_SYMBOL int w_connect(SOCKET socket, struct sockaddr_in *sockAddr, int size);

EXPORT_SYMBOL int w_listen(SOCKET socket, int backlog);

EXPORT_SYMBOL int w_recv(SOCKET socket, char *buf, int len);

EXPORT_SYMBOL int w_send(SOCKET socket, char *buf, int len);

EXPORT_SYMBOL int w_close_socket(SOCKET socket);

EXPORT_SYMBOL int w_shutdown_write(SOCKET socket);

EXPORT_SYMBOL int w_get_last_error();

EXPORT_SYMBOL int w_clean_up();

#endif