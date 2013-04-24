echo "cd Debug_64/"
cd Debug_64/

echo "make clean"
make clean

echo "make all"
make all

echo "cd .."
cd ..

 

echo "cp -f conf/master.conf.std conf/master.conf "
cp -f conf/master.conf.std conf/master.conf

echo "mv Debug_64/kf1304_master kf1304_master"
mv Debug_64/kf1304_master kf1304_master

 
echo "./master_run.sh"
./master_run.sh
