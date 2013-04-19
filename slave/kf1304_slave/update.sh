rm -f slave.tar.gz 
wget -O slave.tar.gz 10.2.112.92:8080/slave/slave-20130418-0.tar.gz 
tar zxvf slave.tar.gz 
cd Debug/ 
./build.sh 
