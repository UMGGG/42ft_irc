#ifndef BOT_HPP
#define BOT_HPP

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

int clnt_sock;

void error_handling(std::string);
void terminate(int signal);
std::string get_date();
std::string check_start(std::string msg);
int command(std::string& message, std::string ch);

#endif
