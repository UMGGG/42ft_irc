#include "Command.hpp"

/*
if (_trailing)
:a!a@127.0.0.1 PART #a :byebye

else
:a!a@127.0.0.1 PART :#a
*/

void Command::PART()
{
    // need more params
    if (_params.size() < 1)
    {
        sendReply(_sender->getSocket(), ERR_NEEDMOREPARAMS(_server->getName(), _sender->getNick(), "PART"));
        return;
    }

    //init parameters
    std::vector<std::string> target;
    std::string buf;
    std::stringstream stream;
    stream.str(_params[0]);
    while (std::getline(stream, buf, ','))
    {
        target.push_back(buf);
        buf.clear();
    }

    for (std::vector<std::string>::iterator it = target.begin(); it != target.end(); ++it)
    {
        // make reply message
        //:root__!root@127.0.0.1 PART :#b
        std::string reply = "";
        if (_trailing.empty())
            reply = RPL_PART(_sender->getNick(), _sender->getUsername(), _sender->getIP(), *it);
        else
            reply = RPL_PARTWITHMSG(_sender->getNick(), _sender->getUsername(), _sender->getIP(), *it, _trailing);

        Channel* channel = _server->getChannel(*it);

        //no such channel - 403
        if ((*it)[0] != '#' || channel == NULL)
        {
            sendReply(_sender->getSocket(), ERR_NOSUCHCHANNEL(_server->getName(), _sender->getNick(), *it));
            continue;
        }
        // not on the channel
        // :irc.local 442 root_ #b :You're not on that channel
        if (!_sender->isJoined(*it))
        {
            sendReply(_sender->getSocket(), ERR_NOTONCHANNEL(_server->getName(), _sender->getNick(), *it));
            continue;
        }

        channel->sendReply(reply);      //send message PART
        _sender->removeJoined(channel); //remove from User::_joined
        channel->removeUser(_sender);   //remove from Channel::_users
        if (channel->empty())
        {
            delete channel;
            _server->removeChannel(channel);
        }
    }

}