echo "cd Debug/"
cd Debug/

echo "make clean"
make clean

echo "make all"
make all

echo "cd .."
cd ..

echo "cp -f conf/master.conf.dev conf/master.conf "
cp -f conf/master.conf.dev conf/master.conf

echo "mv Debug_64/kf1304_master kf1304_master"
mv Debug_64/kf1304_master kf1304_master

 
echo "./kf1304_master"
./kf1304_master
