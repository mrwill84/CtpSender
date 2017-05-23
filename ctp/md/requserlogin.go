package md

import (
	"log"

	"git.dev.com/ctp_sender/ctp/async"

	"fmt"

	"github.com/mrwill84/goctp"
)

func (p *GoCThostFtdcMdSpi) ReqUserLogin() {
	id := p.Client.GetMdRequestID()
	obj, err := async.Alloc(id, func() {
		log.Println("GoCThostFtdcMdSpi.ReqUserLogin.")
		req := goctp.NewCThostFtdcReqUserLoginField()
		req.SetBrokerID(p.Client.BrokerID)
		req.SetUserID(p.Client.InvestorID)
		req.SetPassword(p.Client.Password)
		requestID := p.Client.GetMdRequestID()
		iResult := p.Client.MdApi.ReqUserLogin(req, p.Client.GetMdRequestID())

		if iResult == 0 {
			log.Println("发送用户登录请求: 成功.")
			return
		}
		log.Println("发送用户登录请求: 失败.")
	})

}

func (p *GoCThostFtdcMdSpi) OnRspUserLogin(pRspUserLogin goctp.CThostFtdcRspUserLoginField, pRspInfo goctp.CThostFtdcRspInfoField, nRequestID int, bIsLast bool) {

	if bIsLast && !p.IsErrorRspInfo(pRspInfo) {
		async.Put(nRequestID, pRspUserLogin, nil)
		return
		//log.Printf("获取当前版本信息: %#v\n", goctp.CThostFtdcTraderApiGetApiVersion())
		//log.Printf("获取当前交易日期: %#v\n", p.Client.MdApi.GetTradingDay())
		//log.Printf("获取用户登录信息: %#v %#v %#v\n", pRspUserLogin.GetLoginTime(), pRspUserLogin.GetSystemName(), pRspUserLogin.GetSessionID())

		//ppInstrumentID := []string{"cu1610", "cu1611", "cu1612", "cu1701", "cu1702", "cu1703", "cu1704", "cu1705", "cu1706"}
		//p.SubscribeMarketData(ppInstrumentID)
		//p.SubscribeForQuoteRsp(ppInstrumentID)
	}
	async.Put(nRequestID, nil, fmt.Errorf("pRspUserLogin error "))
}
