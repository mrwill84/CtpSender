#include "csocket.h"
int main(){
    int sockdf=create_socket();
    if (sockdf!=-1){
        bool ret = connect_to(sockdf, "10.10.0.1",21234);
        if (ret == true) {
            const char* buf="test" ;
            send_to(sockdf,buf,5);
        }
    }
    return 0;
}