#include "Command.hpp"

void Command::PONG()
{
	sendReply(_sender->getSocket(), (_sender->getServer()->getName(), _sender->getIP()));
}
