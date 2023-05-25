#ifndef REPLY_HPP
#define REPLY_HPP

//user connect
#define RPL_WELCOME(nick)	":IRC 001" + nick + ":Welcome to the Internet Relay Chat" + name + "\r\n"
#define RPL_YOURHOST(nick)	":IRC 002" + nick + ":Your host is irc, running version 1.0\r\n"
#define RPL_CREATED(nick)	":IRC 003" + nick + "::This server was created on MacOS\r\n"
// #define RPL_MYINFO(nick)	":IRC 004" + nick + ":<servername> <version> <available user modes> <available channel modes>"

#define RPL_PRIVMSG(nick, name, ip, receiver, message)	":" + nick + "!" + name + "@" + ip + " PRIVMSG " + receiver + " " + message + "\r\n"
#define RPL_NOTICE(nick, name, ip, receiver, message)	":" + nick + "!" + name + "@" + ip + " NOTICE " + receiver + " " + message + "\r\n"

#define RPL_QUIT(nick, name, ip, msg) ":" + nick + "!" +

#define RPL_PART(nick, name, ip, channel)	":" + nick + "!" + name + "@" + ip + " PART " + channel + "\r\n"
#define RPL_PART(nick, name, ip, channel, message)	":" + nick + "!" + name + "@" + ip + " PART " + channel + " :" + message + "\r\n"

#define RPL_PONG(nick, name, ip, token	)	":" + nick + "!" + name + "@" + ip + " PONG :" + ip + "\r\n"

#define ERR_PASSWDMISMATCH(ip, user)	":" + ip + " 464 " + user + " :Password incorrect.\r\n"

#endif
