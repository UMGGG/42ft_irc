#include "Command.hpp"

// MODE #tradis +k-k+ki 4242 1234 4343
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
    if (_params.size() == 1)
    {
        // if (no such channel)
        //:irc.local 403 qwer #a :No such channel

        // reply (current channel's mode)
        // :irc.local 324 qwer #tradis +knt :<key>
        // :irc.local 329 qwer #tradis :1685442445
        return;
    }

    //todo
    std::string modes = _params[1]; //+k-k+ki
    bool sign; // true == '+', false == '-'

    std::vector<std::string> mode_params; // 4242 1234 4343
    // int param_idx = 0; // param_idx[0] == "4242", [1] == "1234", [2] == "4343"

    // if there is more parameters following mode_option(_parmas[1])
    if (_params.size() > 2)
    {
        // init parameters for each mode
        for (std::vector<std::string>::iterator it = _params.begin() + 2; it != _params.end(); ++it)
            mode_params.push_back(*it);
    }

    // todo - set modes
    for (size_t i = 0; i < modes.length(); ++i)
    {
        if (modes[i] == '+')
        {
            sign = true;
            continue;
        }

        if (modes[i] == '-')
        {
            sign = false;
            continue;
        }

        if (modes[i] == 'i')
        {
            if (sign)
            {
                //set i
            }
            else
            {
                //remove i
            }
            continue;
        }

        if (modes[i] == 'k')
        {
            if (sign) // sing == '+'
            {
                // set k
            }
            else
            {
                // remove k
            }
            continue;
        }

        if (modes[i] == 'l')
        {
            if (sign)
            {

            }
            else
            {

            }
            continue;
        }

        if (modes[i] == 'o')
        {
            if (sign)
            {

            }
            else
            {

            }
            continue;
        }

        if (modes[i] == 't')
        {
            if (sign)
            {

            }
            else
            {

            }
        }
    }
}