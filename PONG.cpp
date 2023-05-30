#include "Command.hpp"

void Command::PONG()
{
	_sender->setPingtime();
	sendReply(_sender->getSocket(), RPL_PONG(_sender->getServer()->getName(), _sender->getIP()));
}
