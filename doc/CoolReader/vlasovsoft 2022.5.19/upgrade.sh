#!/bin/sh

upgrade/upgrade -stylesheet $STYLESHEET

. $ROOT/wifi_off.sh

if [ -d $ROOT/../vlasovsoft.new ]; then
    echo exit > $VLASOVSOFT_FIFO1
fi

