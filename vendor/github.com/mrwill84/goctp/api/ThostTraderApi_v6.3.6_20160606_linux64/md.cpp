#include <cstring>
#include <iostream>
#include "ThostFtdcMdApi.h"



class GoMdSpi: public CThostFtdcMdSpi{
    public:
        GoMdSpi(CThostFtdcMdApi * papi):m_api(papi){
             std::cout<<"GoMdSpi" <<std::endl;
        }
        virtual void OnFrontConnected(){
            std::cout<<"OnFrontConnected" <<std::endl;
            CThostFtdcReqUserLoginField field;
            std::strcpy(field.BrokerID,"0189") ;
            std::strcpy(field.UserID,"2000052") ;
            std::strcpy(field.Password,"276988");
            m_api->ReqUserLogin(&field,10);
            //m_api->SubscribeMarketData(context,2);
        }
        virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
            std::cout<<"OnRspUserLogin"<<nRequestID<<std::endl;

            std::ifstream filein("instrument.csv");

            for (std::string line; std::getline(filein, line); )
            {
               char buf[1][64]={0};
               std::strcpy(buf[0],line.c_str());
               m_api->SubscribeMarketData((char**)&line.c_str(),1);
            }
        }
        virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
           // std::cout<<pDepthMarketData->InstrumentID<<std::endl;
        };
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