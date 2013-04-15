echo g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Cpp2mysql.d" -MT"src/Cpp2mysql.d" -o "src/Cpp2mysql.o" "../src/Cpp2mysql.cpp"

echo g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/GlobalHelper.d" -MT"src/GlobalHelper.d" -o "src/GlobalHelper.o" "../src/GlobalHelper.cpp"

echo g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/kf1304_master.d" -MT"src/kf1304_master.d" -o "src/kf1304_master.o" "../src/kf1304_master.cpp"

echo g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/master.d" -MT"src/master.d" -o "src/master.o" "../src/master.cpp"

echo g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/slaver.d" -MT"src/slaver.d" -o "src/slaver.o" "../src/slaver.cpp"

echo g++  -o "kf1304_master"  ./src/Cpp2mysql.o ./src/GlobalHelper.o ./src/kf1304_master.o ./src/master.o ./src/slaver.o   -lmysqlclient
 
echo cd ..
echo cp Debug/kf1304_master kf1304_master
echo "ok ./kf1304_master"
