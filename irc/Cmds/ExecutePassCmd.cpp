#include "CmdExecuter.hpp"
#include "../Base/Server.hpp"
#include "../Base/ClientProxy.hpp"

void CmdExecuter::ExecutePassCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server)
{
    // make sure the pass cmd has at least one param
    if(cmdEntry.args.size() < 1)
    {
        server.SendDataToClient(client, IRCResponseCreator::NeedMoreParams("PASS"));
        return;
    }

    // check if password already already set
    if (client.IsRegistered())
    {
        server.SendDataToClient(client, IRCResponseCreator::AlreadyRegistered());
        return;
    }

    std::string password = cmdEntry.args[0];

    // I guess we need some pass protocol !! [ FOR LATER ]
    if(server.IsPasswordCorrect(password))
    {
        client.SetHasAuthedTo(true);
        // IRCLOG("Client with IP " + client.GetIPAddress() + " has been authed");
        // server.SendDataToClient(client, ": Password is correct, you are now authed");
    }
    else
    {
        server.SendDataToClient(client, IRCResponseCreator::PasswordMismatch());
    }
}