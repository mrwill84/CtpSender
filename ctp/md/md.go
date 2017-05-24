package md

import (
	"flag"
	"fmt"
	"log"

	"github.com/mrwill84/CtpSender/ctp/async"
	"github.com/mrwill84/goctp"
)

var (
	broker_id    = flag.String("BrokerID", "9999", "经纪公司编号,SimNow BrokerID统一为：9999")
	investor_id  = flag.String("InvestorID", "<InvestorID>", "交易用户代码")
	pass_word    = flag.String("Password", "<Password>", "交易用户密码")
	market_front = flag.String("MarketFront", "tcp://180.168.146.187:10031", "行情前置,SimNow的测试环境: tcp://180.168.146.187:10031")
	trade_front  = flag.String("TradeFront", "tcp://180.168.146.187:10030", "交易前置,SimNow的测试环境: tcp://180.168.146.187:10030")
)

var CTP GoCTPClient

type GoCTPClient struct {
	BrokerID   string
	InvestorID string
	Password   string

	MdFront string
	MdApi   goctp.CThostFtdcMdApi

	TraderFront string
	TraderApi   goctp.CThostFtdcTraderApi

	MdRequestID     int
	TraderRequestID int
	FrontendConnent chan bool
}

func (g *GoCTPClient) GetMdRequestID() int {
	g.MdRequestID += 1
	return g.MdRequestID
}

func NewDirectorCThostFtdcMdSpi(v interface{}) goctp.CThostFtdcMdSpi {

	return goctp.NewDirectorCThostFtdcMdSpi(v)

}

type GoCThostFtdcMdSpi struct {
	Client GoCTPClient
}

func (p *GoCThostFtdcMdSpi) OnRspError(pRspInfo goctp.CThostFtdcRspInfoField, nRequestID int, bIsLast bool) {
	log.Println("GoCThostFtdcMdSpi.OnRspError.")
	p.IsErrorRspInfo(pRspInfo)
}

func (p *GoCThostFtdcMdSpi) OnFrontDisconnected(nReason int) {
	log.Printf("GoCThostFtdcMdSpi.OnFrontDisconnected: %#v\n", nReason)
}

func (p *GoCThostFtdcMdSpi) OnHeartBeatWarning(nTimeLapse int) {
	log.Printf("GoCThostFtdcMdSpi.OnHeartBeatWarning: %v", nTimeLapse)

}

func (p *GoCThostFtdcMdSpi) OnFrontConnected() {
	log.Println("GoCThostFtdcMdSpi.OnFrontConnected.")
	if CTP.FrontendConnent {
		CTP.FrontendConnent <- true
		return
	}
	panic("never")
	//p.ReqUserLogin()
}

func (p *GoCThostFtdcMdSpi) IsErrorRspInfo(pRspInfo goctp.CThostFtdcRspInfoField) bool {
	// 如果ErrorID != 0, 说明收到了错误的响应
	bResult := (pRspInfo.GetErrorID() != 0)
	if bResult {
		log.Printf("ErrorID=%v ErrorMsg=%v\n", pRspInfo.GetErrorID(), pRspInfo.GetErrorMsg())
	}
	return bResult
}

func (p *GoCThostFtdcMdSpi) ReqUserLogin() (interface{}, error) {
	id := p.Client.GetMdRequestID()
	obj, err := async.Alloc(id, func() {
		log.Println("GoCThostFtdcMdSpi.ReqUserLogin.")
		req := goctp.NewCThostFtdcReqUserLoginField()
		req.SetBrokerID(p.Client.BrokerID)
		req.SetUserID(p.Client.InvestorID)
		req.SetPassword(p.Client.Password)
		requestID := id
		iResult := p.Client.MdApi.ReqUserLogin(req, requestID)

		if iResult == 0 {
			log.Println("发送用户登录请求: 成功.")
			return
		}
		log.Println("发送用户登录请求: 失败.")
	})
	return obj, err
}

func (p *GoCThostFtdcMdSpi) OnRspUserLogin(pRspUserLogin goctp.CThostFtdcRspUserLoginField, pRspInfo goctp.CThostFtdcRspInfoField, nRequestID int, bIsLast bool) {

	if bIsLast && !p.IsErrorRspInfo(pRspInfo) {
		async.Put(nRequestID, pRspUserLogin, nil)
		return
	}
	async.Put(nRequestID, nil, fmt.Errorf("pRspUserLogin error "))
}
