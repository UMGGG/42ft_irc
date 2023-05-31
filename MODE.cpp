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
        // iklot
        std::string message = "";
        std::vector<std::string> param_message;
        std::string list = "+";
        std::string key = "";
        std::string limit = "";

        int mode = channel->getMode();

        if (mode & MODE_I)
            list += "i";
        if (mode & MODE_K)
        {
            list += "k";
            if (_sender->isJoined(_params[0])) // _sender who is joined can get key
                key = channel->getKey();
            else
                key = "<key>"; // hide key to not on this channel
        }
        if (mode & MODE_L)
        {
            std::stringstream sstream;
            sstream << channel->getLimit();

            list += "l";
            limit = sstream.str();
        }
        if (mode & MODE_T)
            list += "t";
        
        param_message.push_back(list);
        if (!key.empty())
            param_message.push_back(key);
        if (!limit.empty())
            param_message.push_back(limit);

        for (std::vector<std::string>::iterator it = param_message.begin(); it != param_message.end(); ++it)
        {
            if (it + 1 != param_message.end())
                message += " " + *it;
            else
                message += " :" + *it;
        }
        sendReply(_sender->getSocket(), RPL_CHANNELMODEIS(_server->getName(), _sender->getNick(), _params[0], message));
        return;
    }

    // not an operator
    if (!channel->isOperator(_sender))
    {
        sendReply(_sender->getSocket(), ERR_CHANOPRIVSNEEDED(_server->getName(), _sender->getNick(), _params[0]));
        return;
    }

    // if command is ...
    // MODE #channel +k-k+ki 4242 1234 4343
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

    // set modes
    for (size_t i = 0; i < modes.length(); ++i)
    {
        switch (modes[i])
        {
            case '+':
            {
                sign = true;
                break;
            }
            case '-':
            {
                sign = false;
                break;
            }
            case 'i':
            {   
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
            }
            case 'k':
            {
                // no parameter
                if ((size_t)param_idx + 1 > mode_params.size())
                {
                    sendReply(_sender->getSocket(), ERR_INVALIDMODEPARAM(_server->getName(), _sender->getNick(), _params[0], "k", "*"));
                    break;
                }

                std::string key = mode_params[param_idx];
                ++param_idx;

                if (sign == true && !(channel->getMode() & MODE_K))
                {
                    success = true;
                    channel->addMode(MODE_K);
                    reply_params[0] += "+k";
                    channel->setKey(key);
                    reply_params.push_back(key);
                }
                else if (sign == false && channel->getMode() & MODE_K)
                {
                    // incorrect key - 467 error
                    if (channel->getKey().compare(key))
                    {
                        sendReply(_sender->getSocket(), ERR_KEYSET(_server->getName(), _sender->getNick(), _params[0]));
                        break;
                    }
                    success = true;
                    channel->removeMode(MODE_K);
                    reply_params[0] += "-k";
                    reply_params.push_back(key);
                }
                break;
            }
            case 'l':
            {
                if (sign == true) // need parameter
                {
                    // no parameter
                    if ((size_t)param_idx + 1 > mode_params.size())
                    {
                        sendReply(_sender->getSocket(), ERR_INVALIDMODEPARAM(_server->getName(), _sender->getNick(), _params[0], "l", "*"));
                        break;
                    }

                    std::string param = mode_params[param_idx];
                    ++param_idx;

                    // set limit
                    int limit = atoi(param.c_str());
                    
                    // if (already set same _limit)
                    if (channel->getMode() & MODE_L && (size_t)limit == channel->getLimit())
                        break;

                    success = true;
                    
                    channel->addMode(MODE_L);
                    reply_params[0] += "+l";

                    channel->setLimit(limit);

                    // std::string s = "";
                    // for (size_t i = 0; i < param.length(); ++i)
                    // {
                    //     if (isdigit(param[i]))
                    //         s += param[i];
                    //     else
                    //         break;
                    // }
                    // if (s.empty())
                    //     s = "0";
                    std::stringstream sstream;
                    sstream << limit;
                    reply_params.push_back(sstream.str());
                }
                else if (sign == false && channel->getMode() & MODE_L) // don't need parameter
                {
                    success = true;
                    channel->removeMode(MODE_L);
                    reply_params[0] += "-l";
                }
                break;
            }
            case 'o':
            {
                // no parameter
                if ((size_t)param_idx + 1 > mode_params.size())
                {
                    sendReply(_sender->getSocket(), ERR_INVALIDMODEPARAM(_server->getName(), _sender->getNick(), _params[0], "o", "*"));
                    break;
                }

                std::string param = mode_params[param_idx];
                ++param_idx;

                User* target = channel->getUser(param);
                if (target == NULL)
                    break;

                if (sign == true && !channel->isOperator(target))
                {
                    success = true;
                    channel->addOperator(target);
                    reply_params[0] += "+o";

                    reply_params.push_back(target->getNick());
                }
                else if (sign == false && channel->isOperator(target))
                {
                    success = true;
                    channel->removeOperator(target);
                    reply_params[0] += "-o";

                    reply_params.push_back(target->getNick());
                }
                break;
            }
            case 't':
            {
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
            }
            default:
            {
                // :irc.local 472 qq 3 :is not a recognised channel mode.
                break;
            }
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