#!/bin/sh

while [ True ]; do
for i in {0..3}
do
echo "\$WRTE,5E:CF:7F:8C:0D:24,"$i"1*12" > /dev/udp/10.42.0.105/1360
echo "\$WRTE,5E:CF:7F:8C:0D:24,"$i"1*12" 
sleep 0.5
done


for i in {0..3}
do
echo "\$WRTE,5E:CF:7F:8C:0D:24,"$i"0*12" > /dev/udp/10.42.0.105/1360
echo "\$WRTE,5E:CF:7F:8C:0D:24,"$i"0*12"
sleep 0.5
done
done






#echo "\$WRTE,5E:CF:7F:8C:0D:24,$i 1*12"

#for i in {0..3}
#do
#echo "\$WRTE,5E:CF:7F:8C:0D:24,$i0*12" > /dev/udp/10.42.0.105/1360
#sleep 1
#done
#echo "\$WRTE,1A:FE:34:DF:10:C8,01*12" > /dev/udp/10.42.0.100/1360
#echo "\$WRTE,1A:FE:34:DF:10:C8,11*12" > /dev/udp/10.42.0.100/1360
#sleep 2

#echo "\$WRTE,5E:CF:7F:8C:0D:24,00*12" > /dev/udp/10.42.0.105/1360
#echo "\$WRTE,5E:CF:7F:8C:0D:24,10*12" > /dev/udp/10.42.0.105/1360
#echo "\$WRTE,5E:CF:7F:8C:0D:24,20*12" > /dev/udp/10.42.0.105/1360
#iecho "\$WRTE,5E:CF:7F:8C:0D:24,30*12" > /dev/udp/10.42.0.105/1360

