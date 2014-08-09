#include "server.h"
extern legacyStatsServer server;
void deleteClient(Client *client) {
	client->deleteMe = true;
}
void reallyDeleteClient(Client *client) {
	boost::unordered_set<Client *>::iterator iterator;
	iterator=server.client_list.begin();
	while(iterator != server.client_list.end()) {
		if(*iterator==client) {
			iterator = server.client_list.erase(iterator);
			delete client;
		} else
		iterator++;

	}
}
Client *getProfile(int profileid) {
	Client *client = NULL;
	boost::unordered_set<Client *>::iterator iterator;
	iterator=server.client_list.begin();
	while(iterator != server.client_list.end()) {
		client = *iterator;
		if(client->getProfileID() == profileid) return client;
		iterator++;

	}
	return NULL;
}
