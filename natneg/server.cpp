#include "server.h"
#include "Client.h"
extern serverInfo server;
void deleteClient(Client *client) {
	client->deleteMe = true;
}
void reallyDeleteClient(Client *client) {
	server.client_list.erase(client);
	delete client;
}
Client *find_user(struct sockaddr_in *peer, int instance) {
	boost::unordered_set<Client *>::iterator iterator=server.client_list.begin();
	boost::unordered_set<Client *>::iterator end=server.client_list.end();
	Client *user;
	struct sockaddr_in *userpeer;
	while(iterator != end) {
		user=*iterator;
		++iterator;
		userpeer = user->getSockAddr();
		if((userpeer->sin_addr.s_addr == peer->sin_addr.s_addr) && (userpeer->sin_port == peer->sin_port) && (instance == 0 || user->getInstance() == instance)) {
			return user;
		}
	}
	return NULL;
}

Client *find_user(uint32_t ip, uint16_t port, int instance) {
	boost::unordered_set<Client *>::iterator iterator=server.client_list.begin();
	boost::unordered_set<Client *>::iterator end=server.client_list.end();
	Client *user;
	while(iterator != end) {
		user=*iterator;
		++iterator;
		if(user->getAddress() == ip && user->getPort() == port && (instance == 0 || user->getInstance() == instance)) {
			return user;
		}
	}
	return NULL;
}
Client *find_user_by_cookie(int cookie, int instance) {
	boost::unordered_set<Client *>::iterator iterator=server.client_list.begin();
	boost::unordered_set<Client *>::iterator end=server.client_list.end();
	Client *user;
	while(iterator != end) {
		user=*iterator;
		++iterator;
		if(user->getCookie() == cookie && (user->getInstance() == instance)) {
			return user;
		}
	}
	return NULL;
}
Client *find_user_by_cookie_index(int cookie, int instance, int index) {
	boost::unordered_set<Client *>::iterator iterator=server.client_list.begin();
	boost::unordered_set<Client *>::iterator end=server.client_list.end();
	Client *user;
	while(iterator != end) {
		user=*iterator;
		++iterator;
		if(user->getCookie() == cookie && (user->getInstance() == instance) && user->getIndex() == index) {
			return user;
		}
	}
	return NULL;
}
