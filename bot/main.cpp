#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <signal.h>
#include <Reply_bot.hpp>

int clnt_sock;

void error_handling(std::string);
void terminate(int signal);
std::string get_date();
int command(std::string& message, std::string ch);

int main(int argc, char *argv[])
{
	int result;
	struct sockaddr_in serv_addr;
	char message[500];
	std::string msg;
	std::string ping_msg = "PING ft_irc\r\n";
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

	std::cin >> ch;
	msg = JOIN_MSG(ch);
	send(clnt_sock, msg.c_str(), msg.size(), 0);
	memset(&msg, 0, sizeof(msg));
	recv(clnt_sock, message, sizeof(message) - 1, 0);
	while(1)
	{
		result = recv(clnt_sock, message, sizeof(message) - 1, MSG_DONTWAIT);
		if (result != -1)
		{
			msg = message;
			std::cout << msg << std::endl;
			result = command(msg, ch);
			if (result == 1)
			{
				close(clnt_sock);
				exit (0);
			}
			memset(&message, 0, sizeof(message));
			memset(&msg, 0, sizeof(msg));
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

std::string get_date()
{
	std::string date;
	time_t curr_time = time(NULL);
	struct tm *local_time = NULL;
	local_time = localtime(&curr_time);
	date = TIMER(std::to_string(local_time->tm_year + 1900), std::to_string(local_time->tm_mon), \
	std::to_string(local_time->tm_mday), std::to_string(local_time->tm_mday), \
	std::to_string(local_time->tm_mday), std::to_string(local_time->tm_mday));
	return (date);
}

int command(std::string& message, std::string ch)
{
	std::string command = "";
	std::vector<std::string> params;
	std::string trailing = "";

	std::stringstream stream;
	std::string buf;
	std::string msg;

	message.erase(message.find("\r\n")); // crlf 제거
	message.erase(0, message.find(' ')); // prefix 제거

	if (message.find(':') != std::string::npos) // _trailing
	{
		trailing.append(message.substr(message.find(':') + 1)); //(exclude ':')
		message.erase(message.find(':')); // message에서 _trailing 제거
	}

	stream.str(message);

	std::getline(stream, buf, ' ');
	command.append(buf); // _command
	buf.clear();

	while(std::getline(stream, buf, ' ')) // _params
	{
		params.push_back(buf);
		buf.clear();
	}

	///
	if (!command.compare("PRIVMSG"))
	{
		if (trailing[0] == '!')
		{
			if (trailing.compare("!time") || trailing.compare("!TIME"))
			{
				msg = TIME_MSG(ch, get_date());
				std::cout << "msg" << std::endl;
				send(clnt_sock, msg.c_str(), msg.size(), 0);
			}
			//...
		}
		else
			return 0;
	}

	if (!command.compare("KICK"))
		return 1; //exit
	return 0;
}
