#include <cstring>
#include <iostream>
#include <fstream>
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
            std::strcpy(field.InvestorID,"2000052") ;
	        m_api->ReqSettlementInfoConfirm(&field, 11);
        }
        virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
              std::cout<<"OnRspSettlementInfoConfirm"<<nRequestID<<std::endl;
            CThostFtdcQryInstrumentField field;
            memset(&field,0,sizeof(CThostFtdcQryInstrumentField));
            m_api->ReqQryInstrument(&field,12);
        }
        virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
            std::cout<<"OnRspQryInstrument"<<nRequestID<<std::endl;
            if (pRspInfo!=NULL){
                std::cout<<pRspInfo->ErrorMsg<<std::endl;
            }
            if (pInstrument) {
                std::ofstream of("instrument.csv",std::ios::trunc);
                of<<pInstrument->InstrumentID<<std::endl;
                std::cout<<pInstrument->InstrumentID<<std::endl;
                if (bIsLast){
                    of.close();
                }
            }
        }
    private:
        CThostFtdcTraderApi * m_api;
};
int main(){
    
    CThostFtdcTraderApi* trader_api=CThostFtdcTraderApi::CreateFtdcTraderApi("");
    GoTraderSpi* pSpi = new GoTraderSpi(trader_api);
    std::cout<<"hello world"<<std::endl;
    trader_api->RegisterSpi(pSpi);                         // 注册事件类
	trader_api->SubscribePublicTopic(THOST_TERT_RESTART /*THOST_TERT_RESTART*/);  // 注册公有流
	trader_api->SubscribePrivateTopic(THOST_TERT_RESTART /*THOST_TERT_RESTART*/); // 注册私有流
	trader_api->RegisterFront((char*)"tcp://101.230.8.33:41205");
	trader_api->Init();

	trader_api->Join();
	trader_api->Release();
    return 0;
}