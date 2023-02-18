#ifndef UTILITIES_H
#define UTILITIES_H

#include <arpa/inet.h>

#define RECEIVE_BUFFER_SIZE 50
#define MESSAGE_BUFFER_SIZE 50
#define ACKNOWLEDGE_MESSAGE "ACKNOWLEDGE_MESSAGE"
#define SEND_MESSAGE_AFFIRMATION  "y"
#define STATUS_MESSAGE_SUCCESS "STATUS_MESSAGE_SUCCESS"
#define STATUS_MESSAGE_FAIL "STATUS_MESSAGE_FAIL"
#define SO_REUSEPORT_OPTION_VALUE 1

//TCP
int create_server_socket(struct sockaddr_in* sock_addr, sa_family_t sa_family, const char * ip_addr, in_port_t port, int type);
int connect_server(struct sockaddr_in* sock_addr, sa_family_t sa_family, const char * ip_addr, in_port_t port, int type);
int receive(int fd, char * receive_buffer, int buffer_size);
int send_message(int fd, char * message);

//TCP
void init_sock_addr_in(struct sockaddr_in* sock_addr, sa_family_t sa_family, const char * ip_addr, in_port_t port);
void read_user_input(char* prompt, char* read_buffer, int read_buffer_size);

#endif
