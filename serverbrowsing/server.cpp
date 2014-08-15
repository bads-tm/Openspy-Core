#include "main.h"
#include "server.h"
#include "Client.h"
#include <qr/structs.h>
#include "structs.h"
extern serverInfo server;
boost::shared_ptr<Client> find_user(struct sockaddr_in *peer) {
	boost::container::stable_vector< boost::shared_ptr<Client> >::iterator iterator, end;
	boost::shared_ptr<Client> user, none;
	struct sockaddr_in *userpeer;
	iterator = server.client_list.begin();
	end = server.client_list.end();
	while(iterator != end) {
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
