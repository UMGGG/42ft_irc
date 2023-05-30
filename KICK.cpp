#include "Command.hpp"

void Command::KICK()
{
	// 내가 채널에 없거나 없는 채널일 때
	if (_sender->getServer()->getChannel(this->_params[0]) == NULL)
	{
		sendReply(_sender->getSocket(), ERR_NOSUCHNICK(_server->getName(), _sender->getNick(), this->_params[0]));
		return ;
	}// 채널에 해당 닉네임이 없을 때
	else if (_sender->getServer()->getChannel(this->_params[0])->getUser(this->_params[1]) == NULL)
	{
		sendReply(_sender->getSocket(), ERR_NOSUCHNICK(_server->getName(), _sender->getNick(), this->_params[1]));
		return ;

	} // op권한이 없을때
	else if (_sender->getServer()->getChannel(this->_params[0])->isOperator(_sender) == false)
	{
		sendReply(_sender->getSocket(), ERR_CHANOPRIVSNEEDED(_server->getName(), _sender->getNick(), this->_params[0]));
		return ;
	}
	// 채널목록에서 유저 제외, 강퇴당했다고 채널 전체 유저에게 메시지
	std::string message = RPL_KICK(_sender->getNick(), _sender->getUsername(), _sender->getIP(), this->_params[0], this->_params[1], this->_trailing);
	_sender->getServer()->getChannel(this->_params[0])->sendReply(message);
	_sender->getServer()->getChannel(this->_params[0])->removeUser(_sender->getServer()->getChannel(this->_params[0])->getUser(this->_params[1]));
}
