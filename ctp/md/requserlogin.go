package md

import (
	"log"

	"github.com/mrwill84/CtpSender/ctp/async"

	"fmt"

	"github.com/mrwill84/goctp"
)

func (p *GoCThostFtdcMdSpi) ReqUserLogin() (interface{}, error) {
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
	return obj, err
}

func (p *GoCThostFtdcMdSpi) OnRspUserLogin(pRspUserLogin goctp.CThostFtdcRspUserLoginField, pRspInfo goctp.CThostFtdcRspInfoField, nRequestID int, bIsLast bool) {

	if bIsLast && !p.IsErrorRspInfo(pRspInfo) {
		async.Put(nRequestID, pRspUserLogin, nil)
		return
	}
	async.Put(nRequestID, nil, fmt.Errorf("pRspUserLogin error "))
}
