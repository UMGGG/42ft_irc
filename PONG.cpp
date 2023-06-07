#include "Command.hpp"

void Command::PONG()
{
	sendReply(_sender->getSocket(), RPL_PONG(_sender->getServer()->getName(), _sender->getIP()));
}
