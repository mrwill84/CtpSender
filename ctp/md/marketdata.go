package md

import (
	"log"

	"github.com/mrwill84/goctp"
)

func (p *GoCThostFtdcMdSpi) OnRtnDepthMarketData(pDepthMarketData goctp.CThostFtdcDepthMarketDataField) {

	log.Println("GoCThostFtdcMdSpi.OnRtnDepthMarketData: ", pDepthMarketData.GetTradingDay(),
		pDepthMarketData.GetInstrumentID(),
		pDepthMarketData.GetExchangeID(),
		pDepthMarketData.GetExchangeInstID(),
		pDepthMarketData.GetLastPrice(),
		pDepthMarketData.GetPreSettlementPrice(),
		pDepthMarketData.GetPreClosePrice(),
		pDepthMarketData.GetPreOpenInterest(),
		pDepthMarketData.GetOpenPrice(),
		pDepthMarketData.GetHighestPrice(),
		pDepthMarketData.GetLowestPrice(),
		pDepthMarketData.GetVolume(),
		pDepthMarketData.GetTurnover(),
		pDepthMarketData.GetOpenInterest())
}
