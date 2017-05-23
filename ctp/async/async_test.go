package async

import "testing"
import "github.com/stretchr/testify/assert"
import "time"
import "fmt"

type ObjS struct {
	test int
}

func Request(id int) (ObjS, error) {
	obj, err := Alloc(id, func() {
		fmt.Println("request")
	})
	if err != nil {
		panic(err)
	}
	if o, ok := obj.(ObjS); ok == true {
		return o, nil
	}
	return ObjS{}, err
}
func Rsponse(id int) {
	time.Sleep(1 * time.Second)
	Put(id, ObjS{test: 9}, nil)
}

func TestAsync(t *testing.T) {
	go Rsponse(0)
	obj, err := Request(0)
	if err != nil {
		panic(err)
	}
	assert.Equal(t, 9, obj.test)

}
