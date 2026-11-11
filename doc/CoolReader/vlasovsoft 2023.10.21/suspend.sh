#!/bin/sh

export PATH=$PATH:/sbin:/usr/sbin

echo "suspend.sh: enter"

handler()
{
    kill -15 $pid
    echo 0 > /sys/power/state-extended
    echo "suspend.sh: killed"
    exit 1
}

trap handler SIGTERM

sleep 10 &
pid=$!
wait $pid

. $ROOT/wifi_off.sh

echo 1 > /sys/power/state-extended
echo "suspend.sh: delay 2s (dragon)"
sleep 2 &
pid=$!
wait $pid

echo "sleeping... zzz-zzz-zzz-zzz :)"
sync
echo mem > /sys/power/state
echo 0 > /sys/power/state-extended

echo "suspend.sh: normal exit"

