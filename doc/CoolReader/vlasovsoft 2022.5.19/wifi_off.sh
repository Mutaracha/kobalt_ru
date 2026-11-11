#!/bin/sh
if lsmod | grep -q sdio_wifi_pwr ; then
    wlarm_le -i $INTERFACE down
    ifconfig $INTERFACE down
    /sbin/rmmod $WIFI_MODULE
    usleep 250000
    /sbin/rmmod sdio_wifi_pwr
    usleep 250000
fi

