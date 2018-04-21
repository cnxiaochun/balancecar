#! /bin/bash
cd `dirname $0` 

# stm32flash Download url
# https://sourceforge.net/projects/stm32flash/

if [ -c tty.wchusbserial1420 ]; then
    ~/tools/stm32flash/stm32flash -i 'rts&-dtr,rts&dtr,:,rts&-dtr' -w ../Release/poise.bin /dev/tty.wchusbserial1420
else
    ~/tools/stm32flash/stm32flash -i 'rts&-dtr,rts&dtr,:,rts&-dtr' -w ../Release/poise.bin /dev/ttyUSB0
fi

