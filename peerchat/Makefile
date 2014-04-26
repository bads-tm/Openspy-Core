start:
	g++ *.cpp -fPIC -L/usr/lib64/mysql -lmysqlclient -Wno-write-strings -shared -o ../openspy/modules/peerchat.so  -I.. ../common/*.cpp ../common/*.c
debug:
	g++ *.cpp -lmysqlclient -Wno-write-strings -ggdb -shared -o ../openspy/modules/peerchat.so -I.. ../common/*.cpp ../common/*.c
