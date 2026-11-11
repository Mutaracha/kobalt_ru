#!/bin/sh
rm -f $TMPDIR/dropbear_usb
. $ROOT/dropbear_off.sh
ifconfig usb0 down
rmmod g_ether
rmmod arcotg_udc
