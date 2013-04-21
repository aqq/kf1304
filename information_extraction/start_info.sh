
echo "cd Debug/"
cd Debug/

echo "make clean"
make clean

echo "make all"
make all

echo "cd .."
cd ..

echo "mv Debug/information_extraction information_extraction"
mv Debug/information_extraction information_extraction
 

echo "./information_extraction"
./information_extraction
