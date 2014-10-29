#ifndef _INC_LOOKUP
#define _INC_LOOKUP
#include "main.h"
#include <common/helpers.h>
#include <common/mysql_helpers.h>
#include <playerspy/gp.h>
#include <boost/format.hpp>
#include <sstream>
void sendError(int sd, char *msg);
void checkEmailValid(MYSQL* conn,int sd,char *buff);
void sendNicks(MYSQL* conn,int sd, char *buff);
void checkNick(MYSQL* conn,int sd, char *buff);
void newUser(MYSQL* conn,int sd, char *buff);
void searchUsers(MYSQL* conn,int sd, char *buff);
void uniqueSearch(int sd, char *buff);
void sendReverseBuddies(MYSQL* conn,int sd,char *msg);
#endif
