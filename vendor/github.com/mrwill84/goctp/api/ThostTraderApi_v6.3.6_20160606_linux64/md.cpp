#include <cstring>
#include <iostream>
#include <chrono>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <inttypes.h>
#include "timecov.h"
#include "ThostFtdcMdApi.h"
#include "csocket.h"
#define MARKET_DATA 0

const char* json_format[]={
    "{ \"tradingTime\":\"%s\",\"timestamp\":%" PRIu64 ", \"recv_timestamp\":%" PRIu64 ", \"instrumentID\":\"%s\", \"exchangeID\":\"%s\",\"ExchangeInstID\":\"%s\", "
    "\"lastPrice\":%g, \"preSettlementPrice\":%g, \"preClosePrice\":%g, \"preOpenInterest\":%g, "
    "\"openPrice\":%g, \"highestPrice\":%g, \"lowestPrice\":%g, \"volume\":%d, \"turnover\":%g, "
    "\"openInterest\":%g, \"closePrice\":%g, \"settlementPrice\":%g, \"upperLimitPrice\":%g, \"lowerLimitPrice\":%g, "
    "\"bidPrice1\":%g, \"bidVolume1\":%d, "
    "\"bidPrice2\":%g, \"bidVolume2\":%d, "
    "\"bidPrice3\":%g, \"bidVolume3\":%d, "
    "\"bidPrice4\":%g, \"bidVolume4\":%d, "
    "\"bidPrice5\":%g, \"bidVolume5\":%d, "
    "\"askPrice1\":%g, \"askVolume1\":%d, "
    "\"askPrice2\":%g, \"askVolume2\":%d, "
    "\"askPrice3\":%g, \"askVolume3\":%d, "
    "\"askPrice4\":%g, \"askVolume4\":%d, "
    "\"askPrice5\":%g, \"askVolume5\":%d, "
    "\"averagePrice\":%g }"
};

class GoMdSpi: public CThostFtdcMdSpi{
    public:
        GoMdSpi(CThostFtdcMdApi * papi):m_api(papi){
             std::cout<<"GoMdSpi" <<std::endl;
             m_sockfd =create_socket();
             std::time_t t = std::time(nullptr);
             std::tm tm = *std::localtime(&t);
             char buf[40];
             std::strftime(buf, sizeof(buf), "%Y%m%d", &tm);
             m_tradedate=buf;
        }
        virtual void OnFrontConnected(){
            std::cout<<"OnFrontConnected" <<std::endl;
            CThostFtdcReqUserLoginField field;
            std::strcpy(field.BrokerID,"0189") ;
            std::strcpy(field.UserID,"2000052") ;
            std::strcpy(field.Password,"276988");
            bool c = connect_to(m_sockfd,"10.10.0.1",21234);
            std::cout<<"connect md server: " <<c <<std::endl;
            m_api->ReqUserLogin(&field,10);
            //m_api->SubscribeMarketData(context,2);
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
            std::cout<<"OnRspUserLogin"<<nRequestID<<std::endl;
            if (IsError(pRspInfo)) return ;
            std::ifstream filein("instrument");

            for (std::string line; std::getline(filein, line); )
            {
               char** ptr = new char*[1];      
               ptr[0]= (char*)line.c_str();
               m_api->SubscribeMarketData(ptr ,1);
               delete []ptr;
            }
        }
        
        virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
           // std::cout<<pDepthMarketData->InstrumentID
           //         << pDepthMarketData->TradingDay
            //        <<pDepthMarketData->UpdateTime<<std::endl;
           std::uint64_t recv_time = utc_now();
           std::string daytime= format_time(m_tradedate,pDepthMarketData->UpdateTime);    
           std::uint64_t unixnano = utc_maketimesmape(daytime,pDepthMarketData->UpdateMillisec);  
            
           char buf[1024]={0};
           sprintf(buf,json_format[0],
           daytime.c_str(),
           unixnano,recv_time
           ,pDepthMarketData->InstrumentID, 
           pDepthMarketData->ExchangeID,
           pDepthMarketData->ExchangeInstID,
           pDepthMarketData->LastPrice,
           pDepthMarketData->PreSettlementPrice ,
           pDepthMarketData->PreClosePrice,
           pDepthMarketData->PreOpenInterest,
           pDepthMarketData->OpenPrice,
           pDepthMarketData->HighestPrice ,
           pDepthMarketData->LowestPrice,
           pDepthMarketData->Volume ,
           pDepthMarketData->Turnover ,
           pDepthMarketData->OpenInterest ,
           pDepthMarketData->ClosePrice ,
           pDepthMarketData->SettlementPrice,
           pDepthMarketData->UpperLimitPrice ,
           pDepthMarketData->LowerLimitPrice ,
          // pDepthMarketData->PreDelta ,pDepthMarketData->CurrDelta,
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
           int len =std::strlen(buf);
           short t = (short) len+4;
           send_to(m_sockfd,(const char*)&t,2);
           t=5;
           send_to(m_sockfd,(const char*)&t,2);
           int rc = send_to(m_sockfd,buf,len);
           //std::cout<<buf<<std::endl;
        };
    private:
        CThostFtdcMdApi * m_api;
        int               m_sockfd;
        std::string       m_tradedate;
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