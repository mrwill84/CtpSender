#include <cstring>
#include <iostream>
#include "ThostFtdcTraderApi.h"

  char* context[] ={};



class GoTraderSpi: public CThostFtdcTraderSpi{
    public:
        GoTraderSpi(CThostFtdcTraderApi * papi):m_api(papi){
             std::cout<<"GoTraderSpi" <<std::endl;
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
            CThostFtdcSettlementInfoConfirmField field;
            std::strcpy(field.BrokerID,"0189");
            std::strcpy(field.UserID,"2000052") ;
	        iResult :=m_api->ReqSettlementInfoConfirm(&field, 11)
        }
        virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
            CThostFtdcQryInstrumentField field;
            m_api->ReqQryInstrument(&field);
        }
        virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
            if (pInstrument) {
                std::cout<<pInstrument->InstrumentID<<std::endl;
            }
        }
    private:
        CThostFtdcTraderApi * m_api;
};
int main(){
    
    auto* trader_api=CThostFtdcTraderApi::CreateFtdcTraderApi("")
    GoTraderSpi* pSpi = new GoMdSpi(api);
    std::cout<<"hello world"<<std::endl;
    trader_api->RegisterSpi(pSpi)                         // 注册事件类
	trader_api->SubscribePublicTopic(0 /*THOST_TERT_RESTART*/)  // 注册公有流
	trader_api->SubscribePrivateTopic(0 /*THOST_TERT_RESTART*/) // 注册私有流
	trader_api->RegisterFront(CTP.TraderFront)
	trader_api->Init()

	trader_api->Join()
	trader_api->Release()
    return 0
}