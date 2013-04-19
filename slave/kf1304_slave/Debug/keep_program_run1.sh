#!/bin/bash
#
 

jmsThread=`ps -ef |   grep kf1304_slave | grep -v "grep"`
echo $jmsThread


count=`ps -ef  | grep kf1304_slave | grep -v "grep" | wc -l`

echo $count 
  if [ $count -gt 0 ]; then
          echo "alive"
  else
     ./kf1304_slave      
    fi
 
