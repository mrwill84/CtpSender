#include <iostream>
#include "ThostFtdcMdApi.h"

class GoMdSpi: public CThostFtdcMdSpi{
    public:
        GoMdSpi(){
             std::cout<<"GoMdSpi" <<std::endl;
        }
        virtual void OnFrontConnected(){
            std::cout<<"OnFrontConnected" <<std::endl;
        }
};
int main(){
    GoMdSpi* pSpi = new GoMdSpi();
    std::cout<<"hello world"<<std::endl;
    CThostFtdcMdApi * api = CThostFtdcMdApi::CreateFtdcMdApi("");
    api->RegisterSpi(pSpi);
	api->RegisterFront((char*)"tcp://101.230.8.33:41213");
	api->Init();

	api->Join();
	api->Release();
    return 0;
}