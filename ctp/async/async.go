package async

import "sync"

type resultChan struct {
	Result chan interface{}
	Err    chan error
}

var (
	rdmtx     sync.RWMutex
	resultMap map[int]*resultChan
)

func init() {
	resultMap = make(map[int]*resultChan)
}

// Alloc is for alloc new chan
func Alloc(requestID int, req func()) (interface{}, error) {
	res := new(resultChan)
	res.Result = make(chan interface{}, 1)
	res.Err = make(chan error, 1)
	rdmtx.Lock()
	resultMap[requestID] = res
	//defer func() {
	//	rdmtx.Lock()
	//	delete(resultMap, requestID)
	//	rdmtx.Unlock()
	//}()
	rdmtx.Unlock()
	go req()
	select {
	case rsp := <-res.Result:
		return rsp, nil
	case err := <-res.Err:
		return nil, err
	}

}

//Put 's for ctp callback
func Put(requestID int, obj interface{}, err error) {
	rdmtx.RLock()
	defer rdmtx.RUnlock()
	if err != nil {
		resultMap[requestID].Err <- err
		return
	}
	resultMap[requestID].Result <- obj
}
