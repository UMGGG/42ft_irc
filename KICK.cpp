#include "Command.hpp"

void Command::KICK()
{
	if (this->_params.size() < 2)
	{
		sendReply(_sender->getSocket(), ERR_NEEDMOREPARAMS(_server->getName(), _sender->getNick(), "KICK"));
		return;
	}// sender is not register
	else if (_sender->getStatus() != CONNECTED)
	{
		sendReply(_sender->getSocket(), ERR_NOTREGISTERED(_server->getName(), _sender->getNick(), "KICK"));
		return;
	}// channel not exist
	else if (_server->getChannel(this->_params[0]) == NULL)
	{
		sendReply(_sender->getSocket(), ERR_NOSUCHNICK(_server->getName(), _sender->getNick(), this->_params[0]));
		return ;
	}
	// target user is not exist at channel
	else if (_server->getChannel(this->_params[0])->getUser(this->_params[1]) == NULL)
	{
		sendReply(_sender->getSocket(), ERR_NOSUCHNICK(_server->getName(), _sender->getNick(), this->_params[1]));
		return ;
	}// sender is not exist at channel
	else if (_sender->isJoined(this->_params[0]) == false)
	{
		sendReply(_sender->getSocket(), ERR_NOTONCHANNEL(_server->getName(), _sender->getNick(), this->_params[0]));
		return ;
	} // sender is not op
	else if (_server->getChannel(this->_params[0])->isOperator(_sender) == false)
	{
		sendReply(_sender->getSocket(), ERR_CHANOPRIVSNEEDED(_server->getName(), _sender->getNick(), this->_params[0]));
		return ;
	}
	std::string message = RPL_KICK(_sender->getNick(), _sender->getUsername(), _sender->getIP(), this->_params[0], this->_params[1], this->_trailing);
	_server->getChannel(this->_params[0])->sendReply(message);
	_server->getUser(this->_params[1])->removeJoined(_server->getChannel(this->_params[0]));
	_server->getChannel(this->_params[0])->removeUser(_server->getChannel(this->_params[0])->getUser(this->_params[1]));
	if (_server->getChannel(this->_params[0])->empty())
	{
		delete _server->getChannel(this->_params[0]);
		_server->removeChannel(_server->getChannel(this->_params[0]));
	}
}
