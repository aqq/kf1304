#!/bin/bash
#
        
#打印出当前的kf1304 进程：grep kf1304_slave 查询的kf1304_slave进程，grep -v "grep" 去掉grep进程
jmsThread=`ps -ef |   grep kf1304_slave | grep -v "grep"`
echo $jmsThread

#查询kf1304_slave进程个数：wc -l 返回行数
count=`ps -ef  | grep kf1304_slave | grep -v "grep" | wc -l`

echo $count 
  if [ $count -gt 0 ]; then
    #若进程还未关闭，则脚本sleep几秒
    echo "slave is alive"    
  else
     ./kf1304_slave      
    fi
    
 
