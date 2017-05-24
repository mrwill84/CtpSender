package md
import (
	"log"

	"github.com/mrwill84/CtpSender/ctp/async"

	"fmt"

	"github.com/mrwill84/goctp"
)
var (
	currentRequestID int 
	subSlice[]string
)
func (p *GoCThostFtdcMdSpi) SubscribeMarketData(instruments []string) {

	id := p.Client.GetMdRequestID()
	obj, err := async.Alloc(id, func() {
		iResult := goctp.SubscribeMarketData(instruments)
		if iResult == 0 {
			log.Println("发送用户登录请求: 成功.")
			return
		}
		log.Println("发送用户登录请求: 失败.")
	}
}
func (p *GoCThostFtdcMdSpi) OnRspSubMarketData(pSpecificInstrument goctp.CThostFtdcSpecificInstrumentField, pRspInfo goctp.CThostFtdcRspInfoField, nRequestID int, bIsLast bool) {
	//log.Printf("GoCThostFtdcMdSpi.OnRspSubMarketData: %#v %#v %#v\n", pSpecificInstrument.GetInstrumentID(), nRequestID, bIsLast)
	if !p.IsErrorRspInfo(pRspInfo) {
		if currentRequestID != nRequestID {
			subSlice = []string{}
		}
		if !bIsLast {
			subSlice = append(subSlice,pSpecificInstrument.GetInstrumentID() )
		}else{
			subSlice = append(subSlice,pSpecificInstrument.GetInstrumentID() )
			currentRequestID = nRequestID
			async.Put(nRequestID, subSlice, nil)
		}
	}
}