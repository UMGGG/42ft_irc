#include "Command.hpp"

void Command::PONG()
{
	_sender->setPingtime();
	sendReply(_sender->getSocket(), RPL_PONG(_sender->getServer()->getName(), _sender->getIP()));
}
// PING이 아닌 그냥 다른 커맨드가 오더라도 핑이 온것처럼 시간을 수정해야함
