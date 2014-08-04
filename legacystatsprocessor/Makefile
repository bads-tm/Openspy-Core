start:
	g++ *.cpp -fPIC -L/usr/lib64/mysql -lmysqlclient -Wno-write-strings -fmudflapth -lmudflapth -Wstack-protector -fstack-protector-all -shared -o ../openspy/modules/legacystatsprocessor.so -I.. ../common/*.cpp ../common/*.c -I/usr/include/lua5.1 -L/usr/include -llua-5.1 ../openspy/config.cpp
debug:
	g++ *.cpp -lmysqlclient -ggdb -shared -o ../openspy/modules/legacystatsprocessor.so -I.. ../common/*.cpp ../common/*.c -I/usr/include/lua5.1 -L/usr/include -llua5.1 ../openspy/config.cpp
