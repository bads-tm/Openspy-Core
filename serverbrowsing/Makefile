start:
	g++ *.cpp -fPIC -L/usr/lib64/mysql -lmysqlclient -Wno-write-strings -fmudflapth -lmudflapth -Wstack-protector -fstack-protector-all -shared -o ../openspy/modules/serverbrowsing.so -I.. ../common/*.cpp ../common/*.c
debug:
	g++ *.cpp -lmysqlclient -Wno-write-strings -ggdb -shared -o ../openspy/modules/serverbrowsing.so -I.. ../common/*.cpp ../common/*.c
