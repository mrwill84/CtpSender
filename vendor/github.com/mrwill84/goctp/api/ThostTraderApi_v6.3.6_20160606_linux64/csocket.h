#ifndef __CSOCKET__H__
#define __CSOCKET__H__
#include <string>
#include <sys/socket.h>

int  create_socket();

bool connect_to(const int & sockfd, const std::string& ip_addr, const int& port);

bool send_to(const int & sockfd, const byte* buffer, int len);

#endif