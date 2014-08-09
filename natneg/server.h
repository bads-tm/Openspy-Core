#ifndef _NN_SERVER_INC
#define _NN_SERVER_INC
#include "main.h"
#include "structs.h"
#include <boost/unordered_set.hpp>
class Client;
typedef struct {
	modLoadOptions *loadOptions;
	boost::unordered_set<Client *> client_list;	
} serverInfo;
void deleteClient(Client *client);
void reallyDeleteClient(Client *client);
Client *find_user(struct sockaddr_in *peer, int instance);
Client *find_user(uint32_t ip, uint16_t port, int instance);
Client *find_user_by_cookie(int cookie, int instance);
Client *find_user_by_cookie_index(int cookie, int instance, int index);
#endif
