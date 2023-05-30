#include "Command.hpp"

void Command::QUIT()
{
    _sender->setStatus(DELETE);

    std::string reply;

    // reply to _sender
    if (_trailing.empty())
        reply = RPL_ERROR(std::string("Closing link"), _sender->getUsername(), _sender->getIP(), "Client exited");
        // reply = "ERROR :Closing link: (" + _sender->getUsername() + "@" + _sender->getIP() + ") [Client exited]\r\n";
    else
        reply = RPL_ERROR(std::string("Closing link"), _sender->getUsername(), _sender->getIP(), "Quit: " + _trailing);
        // reply = "ERROR :Closing link: (" + _sender->getUsername() + "@" + _sender->getIP() + ") [Quit: " + _trailing + "]\r\n";
    sendReply(_sender->getSocket(), reply);

    // reply to _joind channel
    if (_trailing.empty())
        reply = RPL_QUIT(_sender->getNick(), _sender->getUsername(), _sender->getIP(), "Client exited");
        // reply = ":" + _sender->getNick() + "!" + _sender->getUsername() + "@" + _sender->getIP() + " QUIT :Client exited\r\n";
        
    else
        reply = RPL_QUIT(_sender->getNick(), _sender->getUsername(), _sender->getIP(), "Quit: " + _trailing);
        // reply = ":" + _sender->getNick() + "!" + _sender->getUsername() + "@" + _sender->getIP() + " QUIT :Quit: " + _trailing + "\r\n";
    _sender->sendNoRepeat(reply);
}