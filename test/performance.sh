#!/bin/sh

for i in {0..20}
do
   echo "\$LIST,1A:FE:34:DF:10:C8,*12" > /dev/udp/10.42.0.208/1360
done
