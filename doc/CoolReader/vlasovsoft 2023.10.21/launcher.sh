#!/bin/sh

if [ -z "$VLASOVSOFT_KSM" ]; then
    export ROOT=/mnt/onboard/.adds/vlasovsoft
fi

LOG=/dev/null

# Uncomment to see log
#LOG=$ROOT/vlasovsoft.log.txt
# Uncomment to see debug from screen
#QWS_SCREEN_DEBUG=:debug
# Uncomment to see debug from mouse
#QWS_MOUSE_DEBUG=:debug
# Uncomment to see debug from keys
#QWS_KEYS_DEBUG=:debug

PLATFORM=freescale
if [ `dd if=/dev/mmcblk0 bs=512 skip=1024 count=1 | grep -c "HW CONFIG"` == 1 ]; then
        CPU=`ntx_hwconfig -s -p /dev/mmcblk0 CPU`
        PLATFORM=$CPU-ntx
        WIFI=`ntx_hwconfig -s -p /dev/mmcblk0 Wifi`
fi
INTERFACE=wlan0
WIFI_MODULE=ar6000
if [ $PLATFORM != freescale ]; then
        INTERFACE=eth0
        WIFI_MODULE=dhd
        if [ x$WIFI == "xRTL8189" ]; then
            WIFI_MODULE=8189fs
        fi
fi
NAME=`/bin/kobo_config.sh 2>/dev/null` 
MODEL_NUMBER=$(cut -f 6 -d ',' /mnt/onboard/.kobo/version | sed -e 's/^[0-]*//')

case $NAME in
 alyssum)
   DEVICE=GLOHD
   QWS_MOUSE_PROTO=KoboTS_h2o
   ;;
 dahlia)
   DEVICE=AURAH2O
   QWS_MOUSE_PROTO=KoboTS_h2o
   ;;
 dragon)
   DEVICE=AURAHD
   QWS_MOUSE_PROTO=KoboTS
   ;;
 phoenix)
   DEVICE=AURA
   QWS_MOUSE_PROTO=KoboTS
   ;;
 kraken)
   DEVICE=GLO
   QWS_MOUSE_PROTO=KoboTS
   ;;
 trilogy)
   DEVICE=TOUCH
   QWS_MOUSE_PROTO=KoboTS
   ;;
 pixie)
   DEVICE=MINI
   QWS_MOUSE_PROTO=KoboTS
   ;;
 pika)
   DEVICE=TOUCH2
   QWS_MOUSE_PROTO=KoboTS_h2o
   ;;
 daylight)
   DEVICE=AURAONE
   QWS_MOUSE_PROTO=KoboTS_h2o
   ;;
 star)
   case $MODEL_NUMBER in
     379) DEVICE=AURA2_v2 ;;
     *)   DEVICE=AURA2_v1 ;;
   esac
   QWS_MOUSE_PROTO=KoboTS_h2o
   ;;
 snow)
   case $MODEL_NUMBER in
     378) DEVICE=AURAH2O2_v2 ;;
     *)   DEVICE=AURAH2O2_v1 ;;
   esac
   QWS_MOUSE_PROTO=KoboTS_h2o2
   ;;
 nova)
   DEVICE=CLARAHD
   QWS_MOUSE_PROTO=KoboTS_h2o2
   ;;
 frost)
   DEVICE=FORMA
   QWS_MOUSE_PROTO=KoboTS_h2o2
   ;;
 storm)
   DEVICE=LIBRA
   QWS_MOUSE_PROTO=KoboTS_h2o2
   ;;
 luna)
   DEVICE=NIA
   QWS_MOUSE_PROTO=KoboTS_h2o2
   ;;
 io)
   DEVICE=LIBRA2
   QWS_MOUSE_PROTO=KoboTS_libra2:mirrorx
   ;;
 *)
   DEVICE=TOUCH
   QWS_MOUSE_PROTO=KoboTS
   ;;
esac

QWS_MOUSE_PROTO=${QWS_MOUSE_PROTO}${QWS_MOUSE_DEBUG}

echo $NAME > $LOG
echo $MODEL_NUMBER >> $LOG
echo $DEVICE >> $LOG
echo $PLATFORM >> $LOG
echo $INTERFACE >> $LOG
echo $WIFI_MODULE >> $LOG

export DEVICE
export PLATFORM 
export INTERFACE 
export WIFI_MODULE
export QWS_MOUSE_PROTO
export TMPDIR=/tmp/vlasovsoft
export LANG=en_US.UTF-8
export VLASOVSOFT_KEY=$ROOT/key
export VLASOVSOFT_KBD=$ROOT/kbd.txt
export VLASOVSOFT_FIFO1=$TMPDIR/fifo1
export VLASOVSOFT_FIFO2=$TMPDIR/fifo2
export VLASOVSOFT_I18N=$ROOT/i18n
export VLASOVSOFT_DICT=$ROOT/dictionary
export QT_PLUGIN_PATH=$ROOT/Qt/plugins
export LD_LIBRARY_PATH=$ROOT/Qt/lib
export QWS_KEYBOARD=KoboKb${QWS_KEYS_DEBUG}
export QWS_DISPLAY=Transformed:KoboFB${QWS_SCREEN_DEBUG}

echo "Display: $QWS_DISPLAY" >> $LOG
echo "Mouse: $QWS_MOUSE_PROTO" >> $LOG
echo "Keyboard: $QWS_KEYBOARD" >> $LOG

export QT_QWS_FONTDIR=$ROOT/fonts
export STYLESHEET=$ROOT/eink.qss

. $ROOT/settings.sh

cd $ROOT

if [ -z "$VLASOVSOFT_KSM" ]; then
    # kill nickel
    killall nickel
    killall sickel
    killall hindenburg
    killall fmon
fi

# make the temporary directory for Qt
mkdir -p $TMPDIR

# make fifos
mkfifo $VLASOVSOFT_FIFO1
mkfifo $VLASOVSOFT_FIFO2

# remount external SD card to read/write
/bin/mount -w -o remount /mnt/sd

# run launcher
if [ -z "$VLASOVSOFT_KSM" ]; then
    echo 0 > /sys/class/graphics/fb0/rotate
fi
$ROOT/launcher -qws -stylesheet $STYLESHEET >> $LOG 2>&1

# remove fifos
rm $VLASOVSOFT_FIFO1
rm $VLASOVSOFT_FIFO2

# check for upgrade
$ROOT/upgrade_finish.sh

if [ -z "$VLASOVSOFT_KSM" ]; then
    if [ -f $ROOT/nickel ]; then
        # remount external SD card to read-only
        /bin/mount -r -o remount /mnt/sd
        . $ROOT/run_nickel.sh
    else
        /sbin/reboot
    fi
fi

