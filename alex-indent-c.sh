#!/bin/bash

indent_cmd="indent -linux -bls -blf -bl -bli0 -nce"
filepath=$(pwd)

workpath="$HOME/.indent_workpath/"

mkdir -p $workpath

filelist=`find ./ -name "*.[ch]" | sed 's/\.\///g'`

for file in $filelist
do
        echo "indent file \"$file\""

        cd $workpath

        tempfile=$(mktemp tmp.XXXXXXXXXX)
        cp $filepath/$file ./$tempfile


        #删除^M
        sed -i "s/\r//g" $tempfile

        rm $filepath/$file -rf
        #windows 下indent读取文件异常
        cat $tempfile | $indent_cmd -o $filepath/$file

        rm -rf $tempfile
        cd $filepath
done
