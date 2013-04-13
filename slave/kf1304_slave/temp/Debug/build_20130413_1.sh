g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/GlobalHelper.d" -MT"src/GlobalHelper.d" -o "src/GlobalHelper.o" "../src/GlobalHelper.cpp"

g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/kf1304_slave.d" -MT"src/kf1304_slave.d" -o "src/kf1304_slave.o" "../src/kf1304_slave.cpp"

g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/slaver.d" -MT"src/slaver.d" -o "src/slaver.o" "../src/slaver.cpp"

g++  -o "kf1304_slave"  ./src/GlobalHelper.o ./src/kf1304_slave.o ./src/slaver.o   
cd ..
cp Debug/kf1304_slave kf1304_slave

