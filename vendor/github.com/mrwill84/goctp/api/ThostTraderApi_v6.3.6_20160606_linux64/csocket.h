#ifndef __CSOCKET__H__
#define __CSOCKET__H__
#include <string>

int  create_socket();

bool connect_to(const int & sockfd, const std::string& ip_addr, const int& port);

bool send_to(const int & sockfd, const char* buffer, int len);

void closefd(int  sockfd);

#endif