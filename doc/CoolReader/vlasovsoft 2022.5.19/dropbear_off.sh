#!/bin/sh
if [[ -f /var/run/dropbear.pid ]] && [[ ! -f $TMPDIR/dropbear_usb ]] && [[ ! -f $TMPDIR/dropbear_wifi ]]; then
    kill `cat /var/run/dropbear.pid`
fi
