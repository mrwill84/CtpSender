#include <cstring>
#include <iostream>
#include <chrono>
#include <sstream>
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
   
           std::string daytime= format_time(pDepthMarketData);
           std::tm t;
           std::istringstream ss(input);
           ss >> std::get_time(&t, "%Y-%m-%D %H:%M:%S");
           time_t t1 =utc_mktime(&t)
           auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
           uint64 unixnano = std::chrono::duration_cast<std::chrono::nano>(tp.time_since_epoch()).count())

           char buf[512]={0};
           sprintf(buf,json_format[0],format_time(pDepthMarketData),unixnano 
           ,pDepthMarketData->InstrumentID, pDepthMarketData->ExchangeID,pDepthMarketData->ExchangeInstID,
           LastPrice,PreSettlementPrice ,PreClosePrice,PreOpenInterest,OpenPrice,HighestPrice ,LowestPrice ,
           Volume ,Turnover ,OpenInterest ,ClosePrice ,SettlementPrice,UpperLimitPrice ,LowerLimitPrice ,
           PreDelta ,CurrDelta,
           BidPrice1, BidVolume1,
           BidPrice2, BidVolume2,
           BidPrice3, BidVolume3,
           BidPrice4, BidVolume4,
           BidPrice5, BidVolume5,
           AskPrice1, AskVolume1,
           AskPrice2, AskVolume2,
           AskPrice3, AskVolume3,
           AskPrice4, AskVolume4,
           AskPrice5, AskVolume5,AveragePrice);

        };
        time_t utc_mktime(struct tm *t)
        {
            return mktime(t) + timezone;
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