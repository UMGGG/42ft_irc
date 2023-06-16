#include "Bot.hpp"
#include "Reply_bot.hpp"

int main(int argc, char *argv[])
{
	// int result;

	struct sockaddr_in serv_addr;
	std::string servername;

	char message[1024];

	std::string ping_msg;
	std::string msg;
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
	sleep(1);
	recv(clnt_sock, message, sizeof(message) - 1, 0);
	msg = message;
	servername = check_start(msg);
	ping_msg = PING_MSG(servername);
	memset(&message, 0, sizeof(message));

	signal(SIGINT, terminate);

	std::cout << "type <channel> <password>" << std::endl;
	std::getline(std::cin, ch);
	msg = JOIN_MSG(ch);
	send(clnt_sock, msg.c_str(), msg.size(), 0);

	msg.clear();
	msg = "";

	// Set socket timeout
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	setsockopt(clnt_sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

	while(1)
	{
		// result = recv(clnt_sock, message, sizeof(message) - 1, MSG_DONTWAIT);
		// if (result != -1)
		// {
		// 	msg = message;
		// 	result = command(msg, ch);
		// 	if (result == 1)
		// 		terminate(2);
		// 	memset(&message, 0, sizeof(message));
		// }
		// if (time(NULL) % 50 == 0)
		// {
		// 	send(clnt_sock, ping_msg.c_str(), ping_msg.size(), 0);
		// 	sleep(1);
		// }

		char buf[512];
		int nread;

		nread = recv(clnt_sock, buf, 512, 0);

		if (nread == 0)
		{
			std::cout << "disconnected" << std::endl;
			//do close??
			exit(1);
		}
		else if (nread == -1)
		{
			send(clnt_sock, ping_msg.c_str(), ping_msg.size(), 0);
			std::cout << "Send: " << ping_msg << std::endl;
		}
		else
			msg.append(buf, nread);

		while (msg.find("\r\n") != std::string::npos)
		{
			std::cout << "Recv: " << msg.substr(0, msg.find("\r\n") + 2) << std::endl;
			if (command(msg, ch))
				terminate(2);
			msg.erase(0, msg.find("\r\n") + 2); // trim used message
		}
	}
}

void error_handling(std::string message)
{
	std::cerr << message << std::endl;
	close(clnt_sock);
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
	std::stringstream year;
	year << local_time->tm_year + 1900;
	std::stringstream month;
	month << local_time->tm_mon;
	std::stringstream day;
	day << local_time->tm_mday;
	std::stringstream hour;
	hour << local_time->tm_hour;
	std::stringstream min;
	min << local_time->tm_min;
	std::stringstream sec;
	sec << local_time->tm_sec;
	date = TIMER(year.str(), month.str(), day.str(), hour.str(), min.str(), sec.str());
	return (date);
}

int command(std::string message, std::string ch)
{
	std::string command = "";
	std::vector<std::string> params;
	std::string trailing = "";

	std::stringstream stream;
	std::string buf;
	std::string msg;

	message.erase(message.find("\r\n")); // crlf 제거
	message.erase(0, message.find(' ') + 1); // prefix 제거

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

	if (!command.compare("PRIVMSG"))
	{
		if (trailing[0] == '!')
		{
			if (!trailing.compare("!time") || !trailing.compare("!TIME"))
			{
				msg = TIME_MSG(ch, get_date());
				send(clnt_sock, msg.c_str(), msg.size(), 0);
				std::cout << "Send: " << msg << std::endl;
			}
		}
		else
			return 0;
	}

	if (!command.compare("KICK"))
		return 1; //exit
	return 0;
}

std::string check_start(std::string msg)
{
	std::string servername = "";
	if (msg[0] == ':')
		servername = msg.substr(1, msg.find(' '));
	else
	{
		servername = msg.substr(0, msg.find(' '));
		if (servername == "ERROR")
			error_handling("Can't access to server");
	}
	return (servername);
}
