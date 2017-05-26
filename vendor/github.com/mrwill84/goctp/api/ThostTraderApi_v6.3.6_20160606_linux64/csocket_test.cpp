#include <string>
#include <iostream>
#include "csocket.h"
int main(){
    int sockdf=create_socket();
    if (sockdf!=-1){
        std::cout<<"create_socket"<<std::endl;

        bool ret = connect_to(sockdf, "10.10.0.1",21234);
        std::cout<<"connect_to: "<<ret <<std::endl;
        if (ret == true) {
            const char* buf="test" ;
            short t = 9;
            send_to(sockdf,(const char*)&t,2);
            t=1;
            send_to(sockdf,(const char*)&t,2);
            int rc = send_to(sockdf,buf,5);
            std::cout<<"send_to: "<<rc <<std::endl;
            closefd(sockdf);
        }
    }
    return 0;
}