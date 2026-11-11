#!/bin/sh 
. $ROOT/dropbear_on.sh
touch $TMPDIR/dropbear_wifi
if [ -f $ROOT/wifi_connect_on.sh ]; then
    $ROOT/wifi_connect_on.sh
fi
wifi_server/wifi_server -stylesheet $STYLESHEET
if [ -f $ROOT/wifi_connect_off.sh ]; then
    $ROOT/wifi_connect_off.sh
fi
rm -f $TMPDIR/dropbear_wifi
. $ROOT/dropbear_off.sh
. $ROOT/wifi_off.sh
