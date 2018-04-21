#! /bin/bash
cd `dirname $0` 

if [ -x "/Applications/SEGGER/JLink/JLinkExe" ]; then
    /Applications/SEGGER/JLink/JLinkExe -CommanderScript swd.txt
else
    ~/tools/JLink/JLinkExe -CommanderScript swd.txt
fi


