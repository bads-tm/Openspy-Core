#include "server.h"
#include "Client.h"
extern serverInfo server;
void deleteClient(Client* client) {
	client->deleteMe = true;
}
void reallyDeleteClient(boost::shared_ptr<Client> client) {
	boost::container::stable_vector< boost::shared_ptr<Client> >::iterator iterator=server.client_list.begin();
	while(iterator != server.client_list.end()) {
		if(*iterator == client) { iterator->reset(); return; }
		++iterator;
	}
}
boost::shared_ptr<Client> find_user(struct sockaddr_in *peer) {
	boost::container::stable_vector< boost::shared_ptr<Client> >::iterator iterator;
	boost::shared_ptr<Client> user, none;
	struct sockaddr_in *userpeer;
	iterator = server.client_list.begin();
	while(iterator != server.client_list.end()) {
		user=*iterator;
		++iterator;
		if(!user) continue;
		userpeer = user->getSockAddr();
		if((userpeer->sin_addr.s_addr == peer->sin_addr.s_addr) && (userpeer->sin_port == peer->sin_port)) {
			return user;
		}
	}
	return none;
}
boost::shared_ptr<Client> find_user(uint32_t ip, uint16_t port) {
	struct sockaddr_in peer;
	peer.sin_addr.s_addr = ip;
	peer.sin_port = port;
	return find_user(&peer);
}
