#!/bin/bash
# /home/andrew/slave/slave_run.sh

jmsThread=`ps -ef |   grep kf1304_slave | grep -v "grep"`
echo $jmsThread

count=`ps -ef  | grep kf1304_slave | grep -v "grep" | wc -l`

echo $count 
  if [ $count -gt 0 ]; then
    echo "slave is alive"    
  else
      echo "run slave now"
      cd /home/andrew/slave 
        
     ./kf1304_slave    

    fi
    