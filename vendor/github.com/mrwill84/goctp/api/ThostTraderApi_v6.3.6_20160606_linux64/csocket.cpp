#include "csocket.h"
#include <sys/socket.h>
#include <netinet/in.h>
int create_socket(){
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
}

bool connect_to(const int & sockfd, const std::string& ip_addr, const int& port){

    struct sockaddr_in sa; // IPv4
    memset(&addr, 0, sizeof(sa));
    inet_pton(AF_INET, ip_addr.c_str(), &(sa.sin_addr)); // IPv4
    sa.sin_family      = AF_INET;
    addr.sin_port      = htons(port);

   /*************************************************/
   /* Connect to the server                         */
   /*************************************************/
   rc = connect(sockfd,
                (struct sockaddr *)&sa,
                sizeof(struct sockaddr_in));
   return rc!=0;
}

bool send_to(const int & sockfd, const byte* buffer, int len){
    int asend = 0 ;
    while(asend !=len ){
        int n = write(sockfd, buffer+ asend, len- asend,0);
        asend += n;
    }
    return asend == len;
}