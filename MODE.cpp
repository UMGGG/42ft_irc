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

    Channel* channel = _server->getChannel(_params[0]);

    if (_params[0][0] != '#') // this is user mode command (ignore)
        return ;

    // no mode option (show current channel's mode)
    if (_params.size() == 1)
    {
        // if (no such channel)
        //:irc.local 403 qwer #a :No such channel
        if (channel == NULL)
        {
            sendReply(_sender->getSocket(), ERR_NOSUCHCHANNEL(_server->getName(), _sender->getNick(), _params[0]));
            return ;
        }

        // reply (current channel's mode)
        // :irc.local 324 qwer #tradis +knt :<key>
        // :irc.local 329 qwer #tradis :1685442445

        return;
    }

    // is operator
    if (!channel->isOperator(_sender))
    {
        // reply err
        //:irc.local 482 root_ #a :You must have channel op access or above to set channel mode i
        sendReply(_sender->getSocket(), ERR_CHANOPRIVSNEEDED(_server->getName(), _sender->getNick(), _params[0]));
        return;
    }

    //todo
    std::string modes = _params[1]; //+k-k+ki
    std::string reply = ":" + _sender->getNick() + "!" + _sender->getUsername() + "@" + _sender->getIP() + " MODE " + _params[0];
    std::vector<std::string> reply_params;
    reply_params.push_back(""); // [0] == modified mode

    bool sign = modes[0] == '+' ? true : false; // true == '+', false == '-'
    bool success = false; // send reply only succes is true;

    std::vector<std::string> mode_params; // 4242 1234 4343
    int param_idx = 0; // param_idx[0] == "4242", [1] == "1234", [2] == "4343"

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
        switch (modes[i])
        {
            case '+':
                sign = true;
                break;

            case '-':
                sign = false;
                break;

            case 'i':
                
                if (sign == true && !(channel->getMode() & MODE_I))
                {
                    success = true;

                    channel->addMode(MODE_I);
                    reply_params[0] += "+i";
                }
                else if (sign == false && channel->getMode() & MODE_I)
                {
                    success = true;

                    channel->removeMode(MODE_I);
                    reply_params[0] += "-i";
                }
                break;

            case 'k':
                if (sign == true)
                {
                }
                else
                {
                    
                }
                break;

            case 'l':
                if (sign == true) // need parameter
                {
                    // if (no param)
                    //:irc.local 696 root #a l * :You must specify a parameter for the limit mode. Syntax: <limit>.
                    if ((size_t)param_idx + 1 > mode_params.size())
                    {
                        sendReply(_sender->getSocket(), ERR_INVALIDMODEPARAM(_server->getName(), _sender->getNick(), _params[0], "l", "*"));
                        break;
                    }

                    // set limit
                    int limit = atoi(mode_params[param_idx].c_str());
                    
                    // if (MODE_L && param == channel->getLimit)
                    // break;
                    if (channel->getMode() & MODE_L && (size_t)limit == channel->getLimit())
                        break;

                    success = true;
                    
                    channel->addMode(MODE_L);
                    reply_params[0] += "+l";

                    channel->setLimit(limit);

                    std::string s = "";
                    for (size_t i = 0; i < mode_params[param_idx].length(); ++i)
                    {
                        if (isdigit(mode_params[param_idx][i]))
                            s += mode_params[param_idx][i];
                        else
                            break;
                    }
                    if (s.empty())
                        s = "0";
                    reply_params.push_back(s);
                    ++param_idx;
                }
                else if (sign == false && channel->getMode() & MODE_L) // don't need parameter
                {
                    success = true;
                    channel->removeMode(MODE_L);
                    reply_params[0] += "-l";
                }
                break;

            case 'o':
                if (sign == true)
                {

                }
                else
                {
                    
                }
                break;

            case 't':
                if (sign == true && !(channel->getMode() & MODE_T))
                {
                    success = true;

                    channel->addMode(MODE_T);
                    reply_params[0] += "+t";
                }
                else if (sign == false && channel->getMode() & MODE_T)
                {
                    success = true;

                    channel->removeMode(MODE_T);
                    reply_params[0] += "-t";
                }
                break;

            default:
                // :irc.local 472 qq 3 :is not a recognised channel mode.
                break;    
        }
    }

    if (success)
    {
        // necessary ??
        //remake reply_params[0];
        // FROM +k+i+t-t-i+o
        // TO   +kit-ti+o

        // append reply_params
        for (std::vector<std::string>::iterator it = reply_params.begin(); it != reply_params.end(); ++it)
        {
            if (it + 1 != reply_params.end())
                reply += " " + *it;
            else
                reply += " :" + *it;
        }
        reply += "\r\n";
        channel->sendReply(reply);
    }
}

//reply with parameters
//:qq!root@127.0.0.1 MODE #tradis -i+t-t+i-k+k-i+io 4242 4242 :root