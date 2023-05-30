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
    // no mode option (show current channel's mode)
    else if (_params.size() == 1)
    {
        // if (no such channel)
        //:irc.local 403 qwer #a :No such channel

        // reply (current channel's mode)
        // :irc.local 324 qwer #tradis +knt :<key>
        // :irc.local 329 qwer #tradis :1685442445
        return;
    }

    //todo
}