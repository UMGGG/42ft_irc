#include "Command.hpp"

// MODE #tradis +k-k+ki 4242 4242 4242
// ㅍㅏ라미터가 필요한 모드의 경우 따라오는 순서대로
// 모드는 처음에 몰려있음

//when mode -k with incorrect key
//:irc.local 467 root #tradis :Channel key already set

void Command::MODE()
{
    // need more params
    if (_params.size() < 1)
    {
        sendReply(_sender->getSocket(), ERR_NEEDMOREPARAMS(_server->getName(), _sender->getNick(), "PART"));
        return;
    }
    // no mode option (ignore)
    else if (_params.size() == 1)
        return;

    //todo
}