#include <cstring>
#include <iostream>
#include <chrono>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include "timecov.h"
#include "ThostFtdcMdApi.h"

#define MARKET_DATA 0

const char* json_format[]={
    "{ tradingTime:\"%s\",timestamp:%lld, recv_timestamp:%lld, instrumentID:\"%s\", exchangeID:\"%s\",ExchangeInstID:\"\", "
    "lastPrice:%g, preSettlementPrice:%g, preClosePrice:%g, preOpenInterest:%g, "
    "openPrice:%g, highestPrice:%g, lowestPrice:%g, volume:%g, turnover:%g, "
    "openInterest:%g, closePrice:%g, settlementPrice:%g, upperLimitPrice:%g, lowerLimitPrice:%g, "
    "bidPrice1:%g, bidVolume1:%g, "
    "bidPrice2:%g, bidVolume2:%g, "
    "bidPrice3:%g, bidVolume3:%g, "
    "bidPrice4:%g, bidVolume4:%g, "
    "bidPrice5:%g, bidVolume5:%g, "
    "askPrice1:%g, askVolume1:%g, "
    "askPrice2:%g, askVolume2:%g, "
    "askPrice3:%g, askVolume3:%g, "
    "askPrice4:%g, askVolume4:%g, "
    "askPrice5:%g, askVolume5:%g, "
    "averagePrice:%g }"
};

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
   
           std::string daytime= format_time(pDepthMarketData->TradingDay,pDepthMarketData->TThostFtdcTimeType);
           std::uint64_t unixnano = utc_maketimesmape(daytime,pDepthMarketData->UpdateMillisec);
           char buf[512]={0};
           sprintf(buf,json_format[0],daytime.c_str(),unixnano,unixnano
           ,pDepthMarketData->InstrumentID, pDepthMarketData->ExchangeID,pDepthMarketData->ExchangeInstID,
           pDepthMarketData->LastPrice,pDepthMarketData->PreSettlementPrice ,pDepthMarketData->PreClosePrice,
           pDepthMarketData->PreOpenInterest,pDepthMarketData->OpenPrice,pDepthMarketData->HighestPrice ,
           pDepthMarketData->LowestPrice,
           pDepthMarketData->Volume ,pDepthMarketData->Turnover ,pDepthMarketData->OpenInterest ,
           pDepthMarketData->ClosePrice ,pDepthMarketData->SettlementPrice,pDepthMarketData->UpperLimitPrice ,
           pDepthMarketData->LowerLimitPrice ,
           pDepthMarketData->PreDelta ,pDepthMarketData->CurrDelta,
           pDepthMarketData->BidPrice1, pDepthMarketData->BidVolume1,
           pDepthMarketData->BidPrice2, pDepthMarketData->BidVolume2,
           pDepthMarketData->BidPrice3, pDepthMarketData->BidVolume3,
           pDepthMarketData->BidPrice4, pDepthMarketData->BidVolume4,
           pDepthMarketData->BidPrice5, pDepthMarketData->BidVolume5,
           pDepthMarketData->AskPrice1, pDepthMarketData->AskVolume1,
           pDepthMarketData->AskPrice2, pDepthMarketData->AskVolume2,
           pDepthMarketData->AskPrice3, pDepthMarketData->AskVolume3,
           pDepthMarketData->AskPrice4, pDepthMarketData->AskVolume4,
           pDepthMarketData->AskPrice5, pDepthMarketData->AskVolume5,pDepthMarketData->AveragePrice);
           std::cout<<buf<<std::endl;
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