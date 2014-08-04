start:
	g++ *.cpp -lmysqlclient -Wno-write-strings -shared -o ../openspy/modules/peerchat.so  -I.. ../common/*.cpp ../common/*.c
debug:
	g++ *.cpp -lmysqlclient -Wno-write-strings -ggdb -fmudflapth -lmudflapth -Wstack-protector -fstack-protector-all -shared -o ../openspy/modules/peerchat.so -I.. ../common/*.cpp ../common/*.c
