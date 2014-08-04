start:
	g++ *.cpp -lmysqlclient -lGeoIP -Wno-write-strings -shared -o ../openspy/modules/qr.so -I.. ../common/*.cpp ../common/*.c
debug:
	g++ *.cpp -lmysqlclient -lGeoIP -Wno-write-strings -ggdb -fmudflapth -lmudflapth -Wstack-protector -fstack-protector-all -shared -o ../openspy/modules/qr.so -I.. ../common/*.cpp ../common/*.c
