
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

echo "mv conf/slave.conf.andrew  conf/slave.conf"
cp -f conf/slave.conf.andrew  conf/slave.conf

echo "./kf1304_slave"
./kf1304_slave
