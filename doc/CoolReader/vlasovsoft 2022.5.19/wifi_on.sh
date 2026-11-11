#!/bin/sh

lsmod | grep -q sdio_wifi_pwr  || insmod /drivers/${PLATFORM}/wifi/sdio_wifi_pwr.ko
lsmod | grep -q ${WIFI_MODULE} || insmod /drivers/${PLATFORM}/wifi/${WIFI_MODULE}.ko

sleep 2
ifconfig $INTERFACE up
wlarm_le -i $INTERFACE up

pidof wpa_supplicant >/dev/null || wpa_supplicant -s -i $INTERFACE -c /etc/wpa_supplicant/wpa_supplicant.conf -C /var/run/wpa_supplicant -B

sleep 1

#if you need dhcp/dynamic uncomment next and comment the 'ip ad below'
/sbin/udhcpc -S -i $INTERFACE -s /etc/udhcpc.d/default.script -t15 -T10 -A3 -b -q >/dev/null 2>&1 &

