#!/bin/bash
 
func()
{
    ./trader.o
    touch finished
}

export BrokerID=0189
export UserID=2000052
export Password=276988
export CTPAddress="tcp://101.230.8.33:41205"
 
func &
sleep 3000
if [ -f finished ]
then
    pkill trader.o
else
    echo "not finished"
fi
rm -f finished
