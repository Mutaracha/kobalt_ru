#!/bin/sh
if [ ! -f /var/run/dropbear.pid ]
then
    $ROOT/sftp.sh
    $ROOT/dropbear/dropbear -E -r $ROOT/dropbear/host.key.rsa -d $ROOT/dropbear/host.key.dss > $ROOT/dropbear/dropbear.log 2>&1
fi
