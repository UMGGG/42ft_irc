#include "Command.hpp"

void Command::KICK()
{
	if (this->_params.size() < 1)
	{
		sendReply(_sender->getSocket(), ERR_NEEDMOREPARAMS(_server->getName(), _sender->getNick(), "KICK"));
		return;
	}// 없는 채널일 때
	else if (_server->getChannel(this->_params[0]) == NULL)
	{
		sendReply(_sender->getSocket(), ERR_NOSUCHNICK(_server->getName(), _sender->getNick(), this->_params[0]));
		return ;
	}
	// 채널에 해당 닉네임이 없을 때
	else if (_server->getChannel(this->_params[0])->getUser(this->_params[1]) == NULL)
	{
		sendReply(_sender->getSocket(), ERR_NOSUCHNICK(_server->getName(), _sender->getNick(), this->_params[1]));
		return ;
	}// 채널에 존재하지않을때
	else if (_sender->isJoined(this->_params[0]) == false)
	{
		sendReply(_sender->getSocket(), ERR_NOTONCHANNEL(_server->getName(), _sender->getNick(), this->_params[0]));
		return ;
	} // op권한이 없을때
	else if (_server->getChannel(this->_params[0])->isOperator(_sender) == false)
	{
		sendReply(_sender->getSocket(), ERR_CHANOPRIVSNEEDED(_server->getName(), _sender->getNick(), this->_params[0]));
		return ;
	}
	// 유저의 채널목록, 채널의 유저목록에서 각각 제외, 강퇴당했다고 채널 전체 유저에게 메시지
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
