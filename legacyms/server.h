#ifndef _SB_SERVER_INC
#define _SB_SERVER_INC
#include "main.h"
class Client;
boost::shared_ptr<Client> find_user(struct sockaddr_in *peer);
boost::shared_ptr<Client> find_user(uint32_t ip, uint16_t port);
#endif
