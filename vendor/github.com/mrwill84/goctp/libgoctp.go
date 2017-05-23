// Copyright 2012 The Go Authors.  All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

package goctp

//#cgo linux LDFLAGS: -fPIC -L. -L$GOPATH/src/github.com/mrwill84/goctp/api/ThostTraderApi_v6.3.6_20160606_linux64 -Wl,-rpath=$GOPATH/src/github.com/mrwill84/goctp/api/ThostTraderApi_v6.3.6_20160606_linux64 -lthostmduserapi -lthosttraderapi -lstdc++
//#cgo linux CPPFLAGS: -fPIC -I. -I$GOPATH/src/github.com/mrwill84/goctp/api/ThostTraderApi_v6.3.6_20160606_linux64
import "C"
