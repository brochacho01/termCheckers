#include "networkUtilities.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int create_server_socket(struct sockaddr_in* sock_addr, sa_family_t sa_family, const char * ip_addr, in_port_t port, int type)
{
  int fd = socket(sa_family, type, 0);
  int optval = SO_REUSEPORT_OPTION_VALUE;
  setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(int));

  init_sock_addr_in(sock_addr, sa_family, ip_addr, port);

  int result = bind(fd, (struct sockaddr *)sock_addr, sizeof(struct sockaddr_in));
  if(result == -1){
    printf("Error on bind, returning\n");
    return -1;
  }
  result = listen(fd, 10);

  return fd;
}

int connect_server(struct sockaddr_in* sock_addr, sa_family_t sa_family, const char * ip_addr, in_port_t port, int type)
{
  int fd = socket(sa_family, type, 0);

  init_sock_addr_in(sock_addr, sa_family, ip_addr, port);

  int result = connect(fd, (struct sockaddr *)sock_addr, sizeof(struct sockaddr_in));
  if(result == -1){
    printf("Error on connect, returning\n");
    return -1;
  }
  return fd;
}

int receive(int fd, char * receive_buffer, int buffer_size)
{
  memset(receive_buffer, 0, buffer_size);
  read(fd, receive_buffer, buffer_size);

  return 0;
}

int send_message(int fd, char * message)
{
  write(fd, message, strlen(message));

  return 0;
}

void init_sock_addr_in(struct sockaddr_in* sock_addr, sa_family_t sa_family, const char * ip_addr, in_port_t port)
{
  memset(sock_addr, 0, sizeof(struct sockaddr_in));

  sock_addr->sin_family = sa_family;
  sock_addr->sin_addr.s_addr = inet_addr(ip_addr);
  sock_addr->sin_port = htons(port);
}

void read_user_input(char* prompt, char* read_buffer, int read_buffer_size)
{
  memset(read_buffer, 0, read_buffer_size);

  printf("%s", prompt);

  fgets(read_buffer, read_buffer_size, stdin);

  read_buffer[strcspn(read_buffer, "\n")] = 0;
}

