start:
	g++ *.cpp -lmysqlclient -Wno-write-strings -shared -o ../openspy/modules/legacyms.so -I.. ../common/*.cpp ../common/*.c -Os
debug:
	g++ *.cpp -lmysqlclient -Wno-write-strings -ggdb -fmudflapth -lmudflapth -Wstack-protector -fstack-protector-all -shared -o ../openspy/modules/legacyms.so -I.. ../common/*.cpp ../common/*.c
