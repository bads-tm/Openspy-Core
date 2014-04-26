start:
	g++ ../common/*.cpp ../common/*.c *.cpp -o openspy -I.. -ldl -lpthread -fPIC -L/usr/lib64/mysql -lmysqlclient -Wwrite-strings
debug:
	g++ *.cpp ../common/*.cpp ../common/*.c -o openspy -I.. -ldl -lpthread -ggdb -lmysqlclient -Wwrite-strings
