#!/bin/sh

echo "upgrade_finish.sh started..."

dirBase="$(dirname "$ROOT")"
dirNew=$dirBase/vlasovsoft.new
if [ -d $dirNew  ]; then

    ver=`cat $ROOT/VERSION`
    dirBak=$dirBase/vlasovsoft_${ver}_bak

    echo "Rename $ROOT to $dirBak"
    mv $ROOT $dirBak

    echo "Rename $dirNew to $ROOT"
    mv $dirNew $ROOT
fi

echo "upgrade_finish.sh finished."

