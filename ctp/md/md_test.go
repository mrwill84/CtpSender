package md

import (
	"fmt"
	"log"
	"testing"

	"time"

	"github.com/mrwill84/goctp"
)

func TestMapGochan(t *testing.T) {

	all := map[int]chan interface{}{}
	all[0] = make(chan interface{}, 1)
	c := all[0]
	delete(all, 0)
	c <- "etet"
	fmt.Println(<-c)
}

func TestMdLogin(t *testing.T) {

	CTP = GoCTPClient{
		BrokerID:   "0189",
		InvestorID: "2000052",
		Password:   "276988",

		MdFront: "tcp://112.65.23.99:41213",
		MdApi:   goctp.CThostFtdcMdApiCreateFtdcMdApi(),
		//TraderFront: *trade_front,
		//TraderApi:   goctp.CThostFtdcTraderApiCreateFtdcTraderApi(),
		MdRequestID:     0,
		TraderRequestID: 0,
		FrontendConnent: make(chan bool, 1),
	}

	log.Printf("客户端配置: %+#v\n", CTP)

	pMdSpi := goctp.NewDirectorCThostFtdcMdSpi(&GoCThostFtdcMdSpi{Client: CTP})

	CTP.MdApi.RegisterSpi(pMdSpi)
	CTP.MdApi.RegisterFront(CTP.MdFront)
	CTP.MdApi.Init()

	go func() {
		CTP.MdApi.Join()
		CTP.MdApi.Release()
	}()
	c := <-CTP.FrontendConnent
	if c == true {
		if spi, ok := pMdSpi.DirectorInterface().(*GoCThostFtdcMdSpi); ok == true {
			rsp, err := spi.ReqUserLogin()
			fmt.Println(rsp, err)
			instruments, err := spi.SubscribeMarketData([]string{"rb1710", "rb1709"})
			sym := instruments.([]string)
			fmt.Println(sym, err)
			time.Sleep(10 * time.Second)
		}
	}

}
