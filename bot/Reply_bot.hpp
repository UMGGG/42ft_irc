#ifndef REPLY_BOT_HPP
#define REPLY_BOT_HPP

#include <time.h>
#include <string>

#define JOIN_MSG(channel) "JOIN " + channel + "\r\n";
#define CHECKIN_MSG(pass) "PASS " + pass + "\r\nNICK TIMEBOT\r\nUSER root root 127.0.0.1 :JJJ\r\n";
#define TIMER(year, month, day, hour, min, sec) year + "년 " + month + "월 " + day + "일 " + hour + "시 " + min + "분 " + sec + "초 ";
#define TIME_MSG(channel, timer) "PRIVMSG " + channel + " :" + timer + "\r\n";
#define PING_MSG(server) "PING " + server + "\r\n";

#endif
