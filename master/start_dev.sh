
echo "cd Debug/"
cd Debug/

echo "make clean"
make clean

echo "make all"
make all

echo "cd .."
cd ..

echo "mv Debug/kf1304_master kf1304_master"
mv Debug/kf1304_master kf1304_master

 
echo "sh slave_run.sh"
sh master_run.sh
 
