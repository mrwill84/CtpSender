package md

import (
	"fmt"
	"log"
	"testing"

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
		BrokerID:   "0",
		InvestorID: "2000052",
		Password:   "276988",

		MdFront: "tcp://101.230.8.33:41213",
		MdApi:   goctp.CThostFtdcMdApiCreateFtdcMdApi(),
		//TraderFront: *trade_front,
		//TraderApi:   goctp.CThostFtdcTraderApiCreateFtdcTraderApi(),
		MdRequestID:     0,
		TraderRequestID: 0,
	}

	log.Printf("客户端配置: %+#v\n", CTP)

	go func() {
		pMdSpi := goctp.NewDirectorCThostFtdcMdSpi(&GoCThostFtdcMdSpi{Client: CTP})

		CTP.MdApi.RegisterSpi(pMdSpi)
		CTP.MdApi.RegisterFront(CTP.MdFront)
		CTP.MdApi.Init()

		CTP.MdApi.Join()
		CTP.MdApi.Release()
	}()
}
