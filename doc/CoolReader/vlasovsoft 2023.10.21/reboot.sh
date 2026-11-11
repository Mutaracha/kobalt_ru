#!/bin/sh
$ROOT/showpic/showpic 10 poweroff.png &
/bin/sleep 2s
/bin/sync
/bin/busybox reboot
