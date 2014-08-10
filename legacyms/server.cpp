#include "server.h"
#include "Client.h"
extern serverInfo server;
void deleteClient(Client* client) {
	client->deleteMe = true;
}
void reallyDeleteClient(boost::shared_ptr<Client> client) {
	if (server.client_list.empty()) return;
	server.client_list.erase(client);
}
boost::shared_ptr<Client> find_user(struct sockaddr_in *peer) {
	boost::shared_ptr<Client> none;
	if (server.client_list.empty()) return none;
	boost::unordered_set< boost::shared_ptr<Client> >::iterator iterator;
	boost::shared_ptr<Client> user;
	struct sockaddr_in *userpeer;
	iterator = server.client_list.begin();
	while(iterator != server.client_list.end()) {
		user=*iterator++;
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
