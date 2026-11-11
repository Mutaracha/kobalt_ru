#!/bin/sh
if [ ! -e /usr/libexec/sftp-server ]; then
    mkdir -p /usr/libexec
    ln -sf $ROOT/dropbear/sftp-server /usr/libexec/sftp-server
fi
