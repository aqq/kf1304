#!/bin/bash
#
min=1
max=100
while [ $min -le $max ]
do   
     
#打印出当前的kf1304 进程：grep kf1304 查询的jboss进程，grep -v "grep" 去掉grep进程
jmsThread=`ps -ef |   grep kf1304_slave | grep -v "grep"`
echo $jmsThread

#查询kf1304_slave进程个数：wc -l 返回行数
count=`ps -ef  | grep kf1304_slave | grep -v "grep" | wc -l`

echo $count 
  if [ $count -gt 0 ]; then
    #若进程还未关闭，则脚本sleep几秒
    echo sleep $sec second the $var time, the kf1304 is still alive
    sleep 5
  else
     ./kf1304_slave
sleep 5      
    fi
    
done  
