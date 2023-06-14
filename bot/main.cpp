#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

#define JOIN_MSG(channel) "JOIN " + channel + "\r\n";
#define CHECKIN_MSG(pass) "PASS " + pass + "\r\nNICK TIMEBOT\r\nUSER root root 127.0.0.1 :JJJ\r\n";

int clnt_sock;

void error_handling(std::string);
void terminate(int signal);

int main(int argc, char *argv[])
{
	int result;
	struct sockaddr_in serv_addr;
	char message[500];
	std::string msg;
	std::string ping_msg = "PING ft_irc\r\n";
	std::string ch_list;
	std::string ch;

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

	std::string pass = argv[3];
	msg = CHECKIN_MSG(pass);
	send(clnt_sock, msg.c_str(), msg.size(), 0);
	memset(&msg, 0, sizeof(msg));

	// servername을 받아와서 저장해줘야함
	signal(SIGINT, terminate);

	std::getline(std::cin, ch_list);
	std::stringstream stream;
	stream.str(ch_list);
	while (stream >> ch)
	{
		msg = JOIN_MSG(ch);
		send(clnt_sock, msg.c_str(), msg.size(), 0);
		memset(&msg, 0, sizeof(msg));
		result = recv(clnt_sock, message, sizeof(message) - 1, 0);
		memset(&message, 0, sizeof(message));
	}

	while(1)
	{
		result = recv(clnt_sock, message, sizeof(message) - 1, MSG_DONTWAIT);
		if (result != -1)
		{
			std::cout << message << std::endl;
			memset(&message, 0, sizeof(message));
		}
		if (time(NULL) % 50 == 0)
		{
			send(clnt_sock, ping_msg.c_str(), ping_msg.size(), 0);
			sleep(1);
		}
		//kick 당하면 exit하기
		//!TIME 들어오면 현재시간 채널로 send 해주기
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
		//std::string exit_msg = "QUIT :Connection closed\r\n";
		//send(clnt_sock, exit_msg.c_str(), exit_msg.size(), 0);
		//sleep(1);
		close(clnt_sock);
		exit (0);
	}
}
