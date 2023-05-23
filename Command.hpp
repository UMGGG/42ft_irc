#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>
#include <iostream>

#define SERVER 1
#define CONNECT 2
#define JOIN 3
#define PART 4
#define QUIT 5
#define EXIT 6
#define DISCONNECT 7
#define MSG 8
#define NOTICE 9
#define KICK 10
#define INVITE 11
#define TOPIC 12
#define MODE 13
#define OP 14
#define DEOP 15

class Command
{
	private:
		std::string msg;
	public:
};

#endif
