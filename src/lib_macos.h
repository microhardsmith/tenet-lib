#ifndef LIB_MACOS_H
#define LIB_MACOS_H

EXPORT_SYMBOL int m_connect_block_code();

EXPORT_SYMBOL int m_send_block_code();

EXPORT_SYMBOL int m_kqueue();

EXPORT_SYMBOL int m_kevent_ctl(int kq, int fd, int16_t filter, uint16_t flags);

EXPORT_SYMBOL int m_kevent_wait(int kq, struct kevent *eventlist, int nevents, struct timespec *timeout);

EXPORT_SYMBOL socklen_t m_address_len();

EXPORT_SYMBOL int m_address(struct sockaddr_in *sockAddr, char *addrStr, socklen_t len);

EXPORT_SYMBOL uint16_t m_port(struct sockaddr_in *sockAddr);

EXPORT_SYMBOL int m_socket_create();

EXPORT_SYMBOL int m_accept(int socket, struct sockaddr_in *clientAddr, socklen_t clientAddrSize);

EXPORT_SYMBOL int m_set_sock_addr(struct sockaddr_in *sockAddr, char *address, uint16_t port);

EXPORT_SYMBOL int m_set_reuse_addr(int socket, int value);

EXPORT_SYMBOL int m_set_keep_alive(int socket, int value);

EXPORT_SYMBOL int m_set_tcp_no_delay(int socket, int value);

EXPORT_SYMBOL int m_get_err_opt(int socket, int *ptr);

EXPORT_SYMBOL int m_set_nonblocking(int socket);

EXPORT_SYMBOL int m_bind(int socket, struct sockaddr_in *sockAddr, socklen_t size);

EXPORT_SYMBOL int m_listen(int socket, int backlog);

EXPORT_SYMBOL int m_err_inprogress();

EXPORT_SYMBOL int m_connect(int socket, struct sockaddr_in *sockAddr, socklen_t size);

EXPORT_SYMBOL ssize_t m_send(int socket, void *buf, size_t len);

EXPORT_SYMBOL ssize_t m_recv(int socket, void *buf, size_t len);

EXPORT_SYMBOL int m_close(int fd);

EXPORT_SYMBOL int m_shutdown_write(int fd);

EXPORT_SYMBOL int m_errno();

#endif