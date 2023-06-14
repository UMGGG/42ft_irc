#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

#define RPL_JOIN(channel) "JOIN " + channel + "\r\n";

int clnt_sock;

void error_handling(std::string);
void terminate(int signal);

int main(int argc, char *argv[])
{
	int result;
	struct sockaddr_in serv_addr;
	char message[500];
	std::string checkin_msg;

	if (argc != 4)
	{
		std::cerr << "usage: ./bot_client <ip> <port> <password>" << std::endl;
		return (1);
	}

	clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (clnt_sock == -1)
		error_handling("socket error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect error");

	checkin_msg += "PASS ";
	checkin_msg += argv[3];
	checkin_msg += "\r\nNICK BOT\r\nUSER root root 127.0.0.1 :JJJ\r\n";
	send(clnt_sock, checkin_msg.c_str(), checkin_msg.size(), 0);
	memset(&checkin_msg, 0, sizeof(checkin_msg));

	std::getline(std::cin, checkin_msg);
	std::string msg = RPL_JOIN(checkin_msg);
	send(clnt_sock, msg.c_str(), msg.size(), 0);
	signal(SIGINT, terminate);
	while(1)
	{
		result = recv(clnt_sock, message, sizeof(message) - 1, 0);
		if (result != -1)
			std::cout << message << std::endl;
		memset(&message, 0, sizeof(message));
		//kick 당하면 exit하기
		//채널이 없는 채널이면 exit하기
	}
}

void error_handling(std::string message)
{
	std::cerr << message << std::endl;
	exit (1);
}

void terminate(int signal)
{
	if (signal == SIGINT)
	{
		std::string exit_msg = "QUIT :leaving\r\n";
		send(clnt_sock, exit_msg.c_str(), exit_msg.size(), 0);
		close(clnt_sock);
		exit (0);
	}
}
