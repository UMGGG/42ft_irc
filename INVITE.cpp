#include "Command.hpp"

void Command::INVITE()
{
	// 없는 채널
	if (_server->getChannel(this->_params[1]) == NULL)
	{
		sendReply(_sender->getSocket(), ERR_NOSUCHNICK(_server->getName(), _sender->getNick(), this->_params[1]));
		return ;
	} // 서버에 해당하는 유저가 없을때
	else if (_server->getUser(this->_params[0]) == NULL)
	{
		sendReply(_sender->getSocket(), ERR_NOSUCHNICK(_server->getName(), _sender->getNick(), this->_params[0]));
		return ;
	} // 채널에 유저가 이미 있을때
	else if (_server->getChannel(this->_params[1])->getUser(this->_params[0]) != NULL)
	{
		sendReply(_sender->getSocket(), ERR_USERONCHANNEL(_sender->getNick(), _server->getName(), this->_params[1], this->_params[0]));
		return ;
	}
	// user의 _invited에 초대 추가하기
	sendReply(_sender->getSocket(), RPL_INVITING(_sender->getNick(), _server->getName(), this->_params[0], this->_params[1]));
	sendReply(_server->getUser(this->_params[0])->getSocket(), RPL_INVITED(_sender->getNick(), _sender->getUsername(), _sender->getIP(), this->_params[0], this->_params[1]));
}
