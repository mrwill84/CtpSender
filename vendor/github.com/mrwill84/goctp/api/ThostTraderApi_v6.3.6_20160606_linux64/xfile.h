#ifndef __XFILE_H__
#define __XFILE_H__

#include <string>
#include <unistd.h>
std::ofstream& with_new_file(const std::string& path){
     
    if (access(path, F_OK)== 0 ){
        std::ofstream f(path,std::ios::app);
    }

}

#endif 