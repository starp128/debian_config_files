#!/bin/sh

if [ $# != 2 ] ; then
    cmd=`basename $0`
    echo "usage: $cmd start_commit end_commit"
    exit -1
fi

echo $1
echo $2

arg1id=`git rev-list -n1 $1`
arg2id=`git rev-list -n1 $2`

tmp=`mktemp`
echo "make $tmp tmp file"
git log --oneline --merges --topo-order $1..$2 > $tmp
if [ $? != 0 ] ; then
    echo err
fi

cat $tmp | while read line
do
    id=`echo $line | cut -d" " -f1`
    tree=`git describe --contains --match $2 $id`
    describe=`echo $line | awk '{$1=""; print}'`

    is_child=""
    base_id=`git merge-base $1 $id`
    if [ $arg1id == $base_id ] ; then
	is_child="yes"
    fi

    output="$id $tree  |   $describe"
    output80=`echo $output | cut -c1-80`
    output80="$output80 $is_child"
    echo $output80
done

rm -v $tmp
