#ifndef _QR_SERVER_INC
#define _QR_SERVER_INC
#include "main.h"
#include "structs.h"
class Client;
void deleteClient(Client* client);
void reallyDeleteClient(boost::shared_ptr<Client> client);
boost::shared_ptr<Client> find_user(struct sockaddr_in *peer);
boost::shared_ptr<Client> find_user(uint32_t ip, uint16_t port);
countryRegion *findCountryByName(char *name);
#endif
