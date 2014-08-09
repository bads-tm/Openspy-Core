#ifndef _INC_GP_STRUCTS
#define _INC_GP_STRUCTS
#define MAX_BUFF 1024
#include "main.h"
#include <boost/unordered_set.hpp>
class Client;
typedef struct {
	boost::unordered_set<Client *> client_list;
	gameInfo *games;
	int num_games;
	MYSQL *conn;
	modLoadOptions *options;
}playerspyServer;
typedef struct {
	int sd;
	struct  sockaddr_in peer;
} clientParams;
#endif
