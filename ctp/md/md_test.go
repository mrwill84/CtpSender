package md

import (
	"fmt"
	"testing"
)

func TestMapGochan(t *testing.T) {

	all := map[int]chan interface{}{}
	all[0] = make(chan interface{}, 1)
	c := all[0]
	delete(all, 0)
	c <- "etet"
	fmt.Println(<-c)
}
