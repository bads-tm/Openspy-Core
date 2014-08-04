start:
	g++ *.cpp -lmysqlclient -Wno-write-strings -shared -o ../openspy/modules/legacystatsprocessor.so -I.. ../common/*.cpp ../common/*.c -I/usr/include/lua5.1 -L/usr/include -llua5.1 ../openspy/config.cpp
debug:
	g++ *.cpp -lmysqlclient -ggdb -fmudflapth -lmudflapth -Wstack-protector -fstack-protector-all -shared -o ../openspy/modules/legacystatsprocessor.so -I.. ../common/*.cpp ../common/*.c -I/usr/include/lua5.1 -L/usr/include -llua5.1 ../openspy/config.cpp
