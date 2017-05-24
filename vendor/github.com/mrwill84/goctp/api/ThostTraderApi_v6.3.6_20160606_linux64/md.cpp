#include <iostream>
#include "ThostFtdcMdApi.h"

const char* context[] ={
    "rb1701",
    "rb1701"
}

class GoMdSpi: public CThostFtdcMdSpi{
    public:
        GoMdSpi(CThostFtdcMdApi * papi):m_api(papi){
             std::cout<<"GoMdSpi" <<std::endl;
        }
        virtual void OnFrontConnected(){
            std::cout<<"OnFrontConnected" <<std::endl;
            m_api->SubscribeMarketData(context)
        }
    private:
        CThostFtdcMdApi * m_api;
};
int main(){
    CThostFtdcMdApi * api = CThostFtdcMdApi::CreateFtdcMdApi();
    GoMdSpi* pSpi = new GoMdSpi(api);
    std::cout<<"hello world"<<std::endl;
    
    api->RegisterSpi(pSpi);
	api->RegisterFront((char*)"tcp://101.230.8.33:41213");
	api->Init();

	api->Join();
	api->Release();
    return 0;
}