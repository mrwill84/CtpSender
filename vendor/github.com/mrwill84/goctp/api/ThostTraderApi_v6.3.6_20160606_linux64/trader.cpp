#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio> 
#include <csignal>
#include <unistd.h>
#include "ThostFtdcTraderApi.h"
 
std::string BrokerID;
std::string UserID;
std::string Password;
std::string CTPAddress;
class GoTraderSpi: public CThostFtdcTraderSpi{
    public:
        GoTraderSpi(CThostFtdcTraderApi * papi):m_api(papi){
        }
        virtual ~GoTraderSpi(){
              m_of.close();
        }

        virtual void OnFrontDisconnected(int nReason){
            std::cerr<<"OnFrontDisconnected, Reason: "<<nReason<<std::endl;
        }

        virtual void OnFrontConnected(){
            std::cout<<"OnFrontConnected"<<std::endl;
            CThostFtdcReqUserLoginField field;
            std::strcpy(field.BrokerID,BrokerID.c_str()) ;
            std::strcpy(field.UserID,UserID.c_str()) ;
            std::strcpy(field.Password,Password.c_str() );
            std::cout <<BrokerID << std::endl
                      <<UserID << std::endl
                      <<Password << std::endl;
            m_api->ReqUserLogin(&field,10);
        }
        bool IsError(CThostFtdcRspInfoField* pRspInfo){
            if (pRspInfo==NULL) { return false; }
            bool bResult = (pRspInfo->ErrorID != 0);
            if (bResult ){
                std::cout<<"ErrorID = " << pRspInfo->ErrorID <<", ErrorMsg = " <<pRspInfo->ErrorMsg<<std::endl;
            }
            return bResult;
        }
        virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
            std::cout<<"OnRspUserLogin"<<std::endl;
            if ( !IsError(pRspInfo) ){
                 CThostFtdcSettlementInfoConfirmField field;
                 std::strcpy(field.BrokerID,BrokerID.c_str());
                 std::strcpy(field.InvestorID,UserID.c_str()) ;
                 std::cout <<BrokerID << std::endl
                      <<UserID << std::endl ;
                 m_api->ReqSettlementInfoConfirm(&field, 11);
            }
        }
        virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
             std::cout<<"OnRspUserLogin"<<std::endl;
             if ( !IsError(pRspInfo) ){
                CThostFtdcQryInstrumentField field;
                memset(&field,0,sizeof(CThostFtdcQryInstrumentField));
                m_api->ReqQryInstrument(&field,12);
             }
        }
        virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
            std::cout<<"OnRspQryInstrument"<<nRequestID<<std::endl;
            if ( !IsError(pRspInfo) ){
                 if (pInstrument) {
                    if (!m_of)
                    {
                        m_of.open("instrument",std::ios::trunc);
                    }
                    m_of<<pInstrument->InstrumentID<<std::endl;
                    if (bIsLast){
                        m_api->Release();
                    }
                }
            }
        }
    private:
        std::ofstream         m_of;// ("instrument",std::ios::app);
        CThostFtdcTraderApi * m_api;
};
void signalHandler( int signum ) {
   std::cout << "Interrupt signal (" << signum << ") received.\n";
 
   exit(signum);  

}
int main(){
    std::signal(SIGINT, signalHandler); 
    BrokerID = getenv("BrokerID");
    UserID = getenv("UserID");
    Password = getenv("Password");
    CTPAddress = getenv("CTPAddress");
    std::cout<<"BrokerID"<<BrokerID<<std::endl;
    std::cout<<"UserID"<<UserID<<std::endl;
    if (Password.length()>0){
        std::cout<<"Password"<<"***"<<std::endl;
    }
    std::cout<<"CTPAddress"<<CTPAddress<<std::endl;
    
 
    CThostFtdcTraderApi* trader_api=CThostFtdcTraderApi::CreateFtdcTraderApi("");
    GoTraderSpi* pSpi = new GoTraderSpi(trader_api);
    trader_api->RegisterSpi(pSpi);                         // 注册事件类
	trader_api->SubscribePublicTopic(THOST_TERT_RESTART /*THOST_TERT_RESTART*/);  // 注册公有流
	trader_api->SubscribePrivateTopic(THOST_TERT_RESTART /*THOST_TERT_RESTART*/); // 注册私有流
	trader_api->RegisterFront((char*)CTPAddress.c_str());
    std::cout<<"start Init"<<std::endl;
	trader_api->Init();
    while( true){
        sleep(2);
    }
    //std::cout<<"start blocking"<<std::endl;
	//trader_api->Join();
	 
    return 0;
}