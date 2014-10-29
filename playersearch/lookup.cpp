#include "lookup.h"
void checkEmailValid(MYSQL* conn,int sd,char *buff) {
	MYSQL_RES *res;
char data[GP_EMAIL_LEN] = { 0 };
char query[256] = { 0 };
    if(!find_param("email", buff, data,sizeof(data))) {
       sendError(sd,"Error recieving request");
       return;	
   }
   mysql_real_escape_string(conn,data,data,strlen(data));
   sprintf(query,"SELECT 1 FROM `users` WHERE `email` = '%s'",data);
   if (mysql_query(conn, query)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
	return;
   }
   res = mysql_store_result(conn);
   int i = mysql_num_rows(res);
   mysql_free_result(res);
   formatSend(sd,true,0,"\\vr\\%d",i);
}
void sendNicks(MYSQL* conn,int sd, char *buff) {
	MYSQL_RES *res;
	MYSQL_ROW row;
    char email[GP_EMAIL_LEN] = { 0 };
    char pass[GP_PASSWORD_LEN] = { 0 };
    char query[1024] = { 0 };
    char gamename[32] = { 0 };
    bool sendUnique = false;
    if(!find_param("email", buff, email, sizeof(email))) {
       sendError(sd,"Error recieving request");
       return;	
    }
    if(find_param("gamename", buff, gamename, sizeof(gamename))) {
       sendUnique = true;//just assume you want it sent
    }
    if(!find_param("pass", buff, pass, sizeof(pass))) {
	if(!find_param("passenc",buff,pass,sizeof(pass))) {
	       sendError(sd,"Error recieving request");
	       return;	
	} else {
		char *dpass;
		int passlen = strlen(pass);
		dpass = (char *)base64_decode((uint8_t *)pass, &passlen);
        	passlen = gspassenc((uint8_t *)dpass);
		strcpy(pass,dpass);
		free(dpass);
	}
   }
   mysql_real_escape_string(conn,email,email,strlen(email));
   mysql_real_escape_string(conn,pass,pass,strlen(pass));
   sprintf_s(query,sizeof(query),"SELECT  `nick`,`uniquenick` FROM  `GameTracker`.`users` INNER JOIN  `GameTracker`.`profiles` ON  `GameTracker`.`profiles`.`userid` = `GameTracker`.`users`.`userid` WHERE  `GameTracker`.`users`.`email` =  '%s' AND  `password` =  '%s'",email,pass);
   if (mysql_query(conn, query)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
	return;
   }
	res = mysql_store_result(conn);
	char *sendmsg;
	char namestr[256] = { 0 };
	int num_rows = mysql_num_rows(res);
	if(num_rows == 0) {
		formatSend(sd,true,0,"\\nr\\\\ndone\\");
	        mysql_free_result(res);
		return;
	}
	int len = 1024;
	sendmsg = (char *)malloc(len);
	memset((void *)sendmsg,0,len);
	sprintf_s(sendmsg,len,"\\nr\\%d",num_rows);
	while ((row = mysql_fetch_row(res)) != NULL) {
		sprintf_s(namestr,sizeof(namestr),"\\nick\\%s",row[0]);
		strncat(sendmsg,namestr,strlen(namestr)%len);
		if(sendUnique) {
			sprintf_s(namestr,sizeof(namestr),"\\uniquenick\\%s",row[1]);
			strncat(sendmsg,namestr,strlen(namestr)%len);
		}
		if(strlen(sendmsg)>(len/2)) {
			len *= 2;
			sendmsg = (char *)realloc(sendmsg,len);			
		}
		
        }
	strcat(sendmsg,"\\ndone\\");
	formatSend(sd,true,0,"%s",sendmsg);
        mysql_free_result(res);
	free((void *)sendmsg);
}
void checkNick(MYSQL* conn,int sd, char *buff) {
	//TODO: add uniquenick support
	char nick[GP_NICK_LEN] = { 0 };
	char email[GP_EMAIL_LEN] = { 0 };
	char pass[GP_PASSWORD_LEN] = { 0 };
	char sendbuff[512] = { 0 };
	int userid,profileid;
	if(!find_param("nick", buff, nick, sizeof(nick))) {
	       sendError(sd,"Error recieving request");
	       return;	
	}
	mysql_real_escape_string(conn,nick,nick,strlen(nick));
	if(!find_param("email", buff, email, sizeof(email))) {
	       sendError(sd,"Error recieving request");
	       return;	
	}
	mysql_real_escape_string(conn,email,email,strlen(email));
	if(!find_param("pass", buff, pass, sizeof(pass))) {
		if(!find_param("passenc",buff,pass,sizeof(pass))) {
		       sendError(sd,"Error recieving request");
		       return;	
		} else {
			char *dpass;
			int passlen = strlen(pass);
			dpass = (char *)base64_decode((uint8_t *)pass, &passlen);
        		passlen = gspassenc((uint8_t *)dpass);
			strcpy(pass,dpass);
			free(dpass);
		}
	}
	mysql_real_escape_string(conn,pass,pass,strlen(pass));
	userid = getUserIDFromEmail(conn,email);
	if(userid == 0) {
		formatSend(sd,true,0,"\\cur\\%d",GP_CHECK_BAD_EMAIL);
		return;
	}
	if(!tryPassword(conn,userid,pass)) {
		formatSend(sd,true,0,"\\cur\\%d",GP_CHECK_BAD_PASSWORD);
		return;
	}
	profileid = getProfileIDFromNickEmail(conn, nick, email);
	if(profileid == 0) {
		formatSend(sd,true,0,"\\cur\\%d",GP_CHECK_BAD_NICK);
		return;
	}
	formatSend(sd,true,0,"\\cur\\0\\pid\\%d",profileid);
	return;
}
void newUser(MYSQL* conn,int sd, char *buff) {
	//TODO: add uniquenick support
	char sendbuff[512] = { 0 };
	char nick[GP_NICK_LEN] = { 0 };
	char email[GP_EMAIL_LEN] = { 0 };
	char pass[GP_PASSWORD_LEN] = { 0 };
	char uniquenick[GP_NICK_LEN] = { 0 };
	int userid,profileid;
	find_param("uniquenick",buff,uniquenick,sizeof(uniquenick));
	if(!find_param("nick", buff, nick, sizeof(nick))) {
	       sendError(sd,"Error recieving request");
	       return;	
	}
	mysql_real_escape_string(conn,nick,nick,strlen(nick));
	if(!find_param("email", buff, email, sizeof(email))) {
	       sendError(sd,"Error recieving request");
	       return;	
	}
	mysql_real_escape_string(conn,email,email,strlen(email));
	if(!find_param("pass", buff, pass, sizeof(pass))) {
		if(!find_param("passenc",buff,pass,sizeof(pass))) {
		       sendError(sd,"Error recieving request");
		       return;	
		} else {
			char *dpass;
			int passlen = strlen(pass);
			dpass = (char *)base64_decode((uint8_t *)pass, &passlen);
        		passlen = gspassenc((uint8_t *)dpass);
			strcpy(pass,dpass);
			free(dpass);
		}
	}
	mysql_real_escape_string(conn,pass,pass,strlen(pass));
	userid = getUserIDFromEmail(conn,email);
	if(userid == 0) {
		userid = registerUser(conn,email,pass);
	}
	if(!tryPassword(conn,userid,pass)) {
		formatSend(sd,true,0,"\\nur\\%d",GP_NEWUSER_BAD_PASSWORD);
		return;
	}
	profileid = getProfileIDFromNickEmail(conn, nick, email);
	if(profileid != 0) {
		formatSend(sd,true,0,"\\nur\\%d\\pid\\%d",GP_NEWUSER_BAD_NICK,profileid);
		return;
	}
	if(!nameValid(nick,false)) {
		formatSend(sd,true,0,"\\nur\\%d",GP_NEWUSER_BAD_NICK);
		return;
	}
	if(uniquenick[0] != 0) {
		profileid = getProfileIDFromUniquenick(conn,uniquenick);
		if(profileid != 0) {
			formatSend(sd,true,0,"\\nur\\%d",GP_NEWUSER_UNIQUENICK_INUSE);
			return;
		}
		if(!nameValid(uniquenick,false)) {
			formatSend(sd,true,0,"\\nur\\%d",GP_NEWUSER_UNIQUENICK_INVALID);
			return;
		}
	}
	if(uniquenick[0] != 0) {
		profileid = makeNewProfileWithUniquenick(conn,nick,uniquenick,userid);
	} else {
		profileid = makeNewProfile(conn,nick,userid);
	}
	if(profileid != 0) {
		formatSend(sd,true,0,"\\nur\\0\\pid\\%d",profileid);
		return;
	}
		
}
void searchUsers(MYSQL* conn,int sd, char *buff) {
	MYSQL_RES *res;
	MYSQL_ROW row;
	int searchnum = 0;
	int sesskey = 0;
	char nick[GP_NICK_LEN] = { 0 };
	char email[GP_EMAIL_LEN] = { 0 };
	char firstname[GP_FIRSTNAME_LEN] = { 0 };
	char lastname[GP_LASTNAME_LEN] = { 0 };
	char icquin[GP_AIMNAME_LEN] = { 0 };
	char skip[GP_AIMNAME_LEN] = { 0 };
	char uniquenick[GP_NICK_LEN] = { 0 };
	char *hideemail = "[hidden]";
	char *trueemail;
	int len = 1024;
	char tbuff[512] = { 0 };
	std::stringstream query;
	bool emailsearch = false;
	int namespaceid = find_paramint("namespaceid",buff);
	memset(&icquin,0,sizeof(icquin));
	if(find_param("email", buff, email, sizeof(email))) {
		mysql_real_escape_string(conn,email,email,strlen(email));
		len += strlen(email);
		searchnum++;
		emailsearch = true;
	}
	if(find_param("uniquenick", buff, uniquenick, sizeof(uniquenick))) {
		mysql_real_escape_string(conn,uniquenick,uniquenick,strlen(uniquenick));
		len += strlen(uniquenick);
		searchnum++;
	}
	if(find_param("nick", buff, nick, sizeof(nick))) {
		mysql_real_escape_string(conn,nick,nick,strlen(nick));
		len += strlen(nick);
		searchnum++;
	}
	if(find_param("firstname", buff, firstname, sizeof(firstname))) {
		mysql_real_escape_string(conn,firstname,firstname,strlen(firstname));
		len += strlen(firstname);
		searchnum++;
	}
	if(find_param("lastname", buff, lastname, sizeof(lastname))) {
		mysql_real_escape_string(conn,lastname,lastname,strlen(lastname));
		len += strlen(lastname);
		searchnum++;
	}
	if(find_param("icquin", buff, icquin, sizeof(icquin))) {
		mysql_real_escape_string(conn,icquin,icquin,strlen(icquin));
		len += strlen(icquin);
		searchnum++;
	}
	if(searchnum == 0) {
	       sendError(sd,"Error recieving request");
	       return;	
	}
	query << "SELECT `profileid`,`nick`,`firstname`,`lastname`,`email`,`uniquenick`,`publicmask` FROM `GameTracker`.`profiles` INNER JOIN `GameTracker`.`users` ON `GameTracker`.`profiles`.`userid` = `GameTracker`.`users`.`userid` WHERE ";
	if(email[0] != 0) {
		query << boost::format("`email` = '%s'") % email;
		if(--searchnum > 1) {
			query << " AND ";
		}
	}
	if(nick[0] != 0) {
		query << boost::format("`nick` = '%s'") % nick;
		if(--searchnum > 1) {
			query << " AND ";
		}
	}
	if(firstname[0] != 0) {
		query << boost::format("`firstname` = '%s'") % firstname;
		if(--searchnum > 1) {
			query << " AND ";
		}
	}
	if(lastname[0] != 0) {
		query << boost::format("`lastname` = '%s'") % lastname;
		if(--searchnum > 1) {
			query << " AND ";
		}
	}
	if(uniquenick[0] != 0) {
		query << boost::format("`uniquenick` = '%s'") % uniquenick;
		if(--searchnum > 1) {
			query << " AND ";
		}
	}
	if(icquin[0] != 0) {
		query << boost::format("`icquin` = '%d'") % atoi(icquin);
	}
//	strcat(query," LIMIT 0,10");
	if (mysql_query(conn, query.str().c_str())) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		return;
	}
	res = mysql_store_result(conn);
	int u_pid,u_hideemail;
	query.str("");
	while ((row = mysql_fetch_row(res)) != NULL) {
		u_pid = atoi(row[0]);
		sprintf_s(nick,sizeof(nick),"%s",row[1]);
		sprintf_s(firstname,sizeof(firstname),"%s",row[2]);
		sprintf_s(lastname,sizeof(lastname),"%s",row[3]);
		sprintf_s(email,sizeof(email),"%s",row[4]);
		if(row[5] != NULL) {
			sprintf_s(uniquenick,sizeof(uniquenick),"%s",row[5]);
		} else uniquenick[0] = 0;
		u_hideemail = ~atoi(row[6])&GP_MASK_EMAIL;
		if(u_hideemail != 0 && !emailsearch) {
			sprintf_s(email,sizeof(email),"[hidden]");
		}
		query << boost::format("\\bsr\\%d\\nick\\%s\\firstname\\%s\\lastname\\%s\\email\\%s\\uniquenick\\%s\\namespaceid\\%d") % u_pid % nick % firstname % lastname % email % uniquenick % namespaceid;
   	}
	query << "\\bsrdone\\";
	formatSend(sd,true,0,"%s",query.str().c_str());
	end:
	mysql_free_result(res);
}
void uniqueSearch(int sd, char *buff) {
	char preferrednick[GP_NICK_LEN] = { 0 };
	if(!find_param("preferrednick", buff, preferrednick, sizeof(preferrednick))) {
		sendError(sd,"Error recieving request");
		return;	
	}
	makeValid(preferrednick);
	std::string sendmsg = "\\us\\7";
	for (char i = 0; i < 7; ++i) {
		sendmsg += "\\nick\\";
		sendmsg += preferrednick;
	}
	sendmsg += "\\usdone\\";
	formatSend(sd,true,0,"%s",sendmsg.c_str());
}
void sendReverseBuddies(MYSQL* conn,int sd,char *msg) {
	MYSQL_RES *res;
	MYSQL_ROW row;
	int len = 1024;
	char tbuff[512] = { 0 };
	#define addVal(x,y,z) if( z == 1 ||strlen(x)) { \
						sprintf_s(tbuff,sizeof(tbuff),"\\%s\\%s",y,x); \
					} \
					if(strlen(tbuff) > (len/2)) { \
						len *= 2; \
						query = (char *)realloc(query,len); \
					} \
					strcat(query,tbuff);
//\others\\sesskey\0\profileid\121812147\namespaceid\1\partnerid\0\gamename\\final
	int profile = find_paramint("profileid",msg);
	char *query = (char *)malloc(len);
sprintf_s(query,len,"SELECT `GameTracker`.`profiles`.`profileid`,`uniquenick`,`email`,`firstname`,`lastname`,`nick`,`publicmask` FROM `Presence`.`buddies` INNER JOIN `GameTracker`.`profiles` ON `Presence`.`buddies`.`profileid` = `GameTracker`.`profiles`.`profileid` INNER JOIN `GameTracker`.`users` ON `GameTracker`.`users`.`userid` = `GameTracker`.`profiles`.`userid` WHERE `Presence`.`buddies`.`targetid` = %d AND `GameTracker`.`users`.`deleted` = 0 AND `GameTracker`.`profiles`.`deleted` = 0",profile);
	printf("query is: %s\n",query);
	if (mysql_query(conn, query)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		free(query);
		return;
	}
	memset(query,0,len);
	strcat(query,"\\others\\");
	res = mysql_store_result(conn);
	while ((row = mysql_fetch_row(res)) != NULL) {
		addVal(row[0],"o",1)
		addVal(row[5],"nick",1)
		addVal(row[3],"first",1)
		addVal(row[4],"last",1)
		if(atoi(row[6])&GP_MASK_EMAIL) {
			addVal(row[2],"email",1)
		} else {
			addVal("[hidden]","email",1)
		}
		addVal(row[1],"uniquenick",0)
	}
	mysql_free_result(res);
	strcat(query,"\\odone\\");
	formatSend(sd,true,0,"%s",query);
	free((void *)query);
	#undef addVal
	
}
void sendError(int sd, char *msg) {
//\error\\errmsg\Error receiving request\final\ /
	formatSend(sd,true,0,"\\error\\\\errmsg\\%s",msg);
        close(sd);
}
