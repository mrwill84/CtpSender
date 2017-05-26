#include "csocket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
int create_socket(){
     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
     return sockfd;
}

bool connect_to(const int & sockfd, const std::string& ip_addr, const int& port){

    struct sockaddr_in sa; // IPv4
    memset(&sa, 0, sizeof(sa));
    inet_pton(AF_INET, ip_addr.c_str(), &(sa.sin_addr)); // IPv4
    sa.sin_family    = AF_INET;
    sa.sin_port      = htons(port);

   /*************************************************/
   /* Connect to the server                         */
   /*************************************************/
   int rc = connect(sockfd,
                (struct sockaddr *)&sa,
                sizeof(struct sockaddr_in));
   return rc==0;
}

bool send_to(const int & sockfd, const char* buffer, int len){
    int asend = 0 ;
    while(asend !=len ){
        int n = send(sockfd, buffer+ asend, len- asend,0);
        if (n==-1) break;
        asend += n;
    }
    return asend == len;
}
void closefd(int  sockfd){
   if (sockfd >= 0) {
      shutdown(sockfd, SHUT_RDWR);// < 0 // secondly, terminate the 'reliable' delivery
      close(sockfd);// < 0 // finally call close()

   }
}