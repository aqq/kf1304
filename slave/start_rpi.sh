
echo "cd Debug/"
cd Debug/

echo "make clean"
make clean

echo "make all"
make all

echo "cd .."
cd ..

echo "mv Debug/kf1304_slave kf1304_slave"
mv Debug/kf1304_slave kf1304_slave

echo "cp -f conf/slave.conf.rpi  conf/slave.conf"
cp -f conf/slave.conf.rpi  conf/slave.conf

echo "sh slave_run.sh"
sh slave_run.sh
