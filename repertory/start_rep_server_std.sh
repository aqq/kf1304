echo "cd Debug/"
cd Debug/

echo "make clean"
make clean

echo "make all"
make all

echo "cd .."
cd ..

echo "cp -f conf/repertory.conf.std conf/repertory.conf "
cp -f conf/repertory.conf.std conf/repertory.conf

echo "cp -f Debug/kf1304_repertory kf1304_repertory"
cp -f Debug/kf1304_repertory kf1304_repertory

 
echo "./start_rep.sh"
./start_rep.sh
