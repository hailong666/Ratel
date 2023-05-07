#!/bin/bash

set -e

#如果没有build目录，创建该目录
if [ ! -d `pwd`/build ]; then
    mkdir `pwd`/build
fi

rm -rf `pwd`/build/*

cd `pwd`/build &&
    cmake .. 
    
#回到项目根目录
cd ..

make
#把头文件拷贝到 /user/include/mymuduo so库拷贝到 /user/lib    
if [ ! -d /usr/include/mymuduo ]; then
    mkdir /usr/include/mymuduo
fi

for header in `ls *.h`
do
    cp $header /usr/include/mymuduo
done

cp `pwd`/lib/libmymuduo.so /usr/lib

#刷新一下动态库缓存
ldconfig