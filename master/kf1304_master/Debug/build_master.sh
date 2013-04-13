g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/GlobalHelper.d" -MT"src/GlobalHelper.d" -o "src/GlobalHelper.o" "../src/GlobalHelper.cpp"

g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/StringHelper.d" -MT"src/StringHelper.d" -o "src/StringHelper.o" "../src/StringHelper.cpp"

g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/TextLogger.d" -MT"src/TextLogger.d" -o "src/TextLogger.o" "../src/TextLogger.cpp"

g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/kf1304_master.d" -MT"src/kf1304_master.d" -o "src/kf1304_master.o" "../src/kf1304_master.cpp"

g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/master.d" -MT"src/master.d" -o "src/master.o" "../src/master.cpp"

g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/slaver.d" -MT"src/slaver.d" -o "src/slaver.o" "../src/slaver.cpp"

g++  -o "kf1304_master"  ./src/GlobalHelper.o ./src/StringHelper.o ./src/TextLogger.o ./src/kf1304_master.o ./src/master.o ./src/slaver.o   
cd ..
cp Debug/kf1304_master kf1304_master
./kf1304_master

