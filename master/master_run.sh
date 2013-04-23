#!/bin/bash
# /home/andrew/master/master_run.sh

count=`ps -ef  | grep kf1304_master | grep -v "grep" | wc -l`

echo $count 
  if [ $count -gt 0 ]; then
    echo "master is alive"    
  else
      echo "run master now"
      cd /home/andrew/master        
     ./kf1304_master    

    fi
   
