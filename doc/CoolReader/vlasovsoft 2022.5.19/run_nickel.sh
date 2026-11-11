#!/bin/sh

PRODUCT=`/bin/kobo_config.sh`;
[ $PRODUCT != trilogy ] && PREFIX=$PRODUCT-

PLATFORM=freescale
if [ `dd if=/dev/mmcblk0 bs=512 skip=1024 count=1 | grep -c "HW CONFIG"` == 1 ]; then
	CPU=`ntx_hwconfig -s -p /dev/mmcblk0 CPU`
	PLATFORM=$CPU-ntx
fi

INTERFACE=wlan0
WIFI_MODULE=ar6000
if [ $PLATFORM != freescale ]; then
	INTERFACE=eth0
	WIFI_MODULE=dhd
fi

export PLATFORM
export PRODUCT
export INTERFACE
export WIFI_MODULE

export NICKEL_HOME=/mnt/onboard/.kobo
export LD_LIBRARY_PATH=/usr/local/Kobo
export WIFI_MODULE_PATH=/drivers/$PLATFORM/wifi/$WIFI_MODULE.ko
export LANG=en_US.UTF-8

export DBUS_SESSION_BUS_ADDRESS=`/bin/dbus-daemon --session --print-address --fork`

. /mnt/onboard/.kobo/on_start.sh

/usr/local/Kobo/hindenburg &
/usr/local/Kobo/nickel -platform kobo -skipFontLoad

if [ -f /usr/local/Kobo/sickel ];
then
    /usr/local/Kobo/sickel -platform kobo:noscreen $@
fi

