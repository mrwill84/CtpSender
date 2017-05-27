#!/bin/bash
 
func()
{
    ./trader.o
    touch finished
}
 
func &
sleep 3000
if [ -f finished ]
then
    pkill trader.o
else
    echo "not finished"
fi
rm -f finished
