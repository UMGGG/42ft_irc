#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"

#define CONNECT 1
#define JOIN 2
#define PART 3
#define QUIT 4
#define EXIT 5
#define DISCONNECT 6
#define MSG 7
#define NOTICE 8
#define KICK 9
#define INVITE 10
#define TOPIC 11
#define MODE 12
#define OP 13
#define DEOP 14

class Command
{
private:
	std::string msg;
	std::vector<Channel>::iterator channel_it;
	std::map<int, User>::iterator user_it;
	std::string prefix;
	std::string command;
	std::vector<std::string> params;
	std::string trailer;

	Server *_server;
	User *_sender;
	// received message
	std::string _command;
	std::vector<std::string> _params;
	std::string _trailing;
	// reply
	std::string _reply;

public:
	//Command(std::string pre, std::string com, std::vector<std::string> par, std::string tra);
	void send_single(int fd, std::string message);
	void send_all(std::vector<int> fds, std::string message);
	// command
	int do_server(int fd, std::string password);
	int do_connect(int fd, std::string password);
	int do_join(const User &user, std::string password);
	int do_msg();

	Command(Server *server, User *sender);
    // test
    void testPrint();
};

#endif
