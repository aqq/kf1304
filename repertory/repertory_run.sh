#!/bin/bash
# /home/andrew/rep

 

count=`ps -ef  | grep kf1304_repertory | grep -v "grep" | wc -l`

echo $count 
  if [ $count -gt 0 ]; then
    echo "rep is alive"    
  else
      echo "run rep  now"
      cd /home/andrew/repertory 
        
     ./kf1304_repertory

    fi
