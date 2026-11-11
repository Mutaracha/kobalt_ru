#!/bin/sh
. $ROOT/dropbear_on.sh
touch $TMPDIR/dropbear_usb
driver_root=/drivers/$PLATFORM/usb/gadget
insmod $driver_root/arcotg_udc.ko
insmod $driver_root/g_ether.ko host_addr=46:0d:9e:67:69:eb dev_addr=46:0d:9e:67:69:ec
ifconfig usb0 192.168.2.101
