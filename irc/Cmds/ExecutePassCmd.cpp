#include "CmdExecuter.hpp"
#include "../Base/Server.hpp"
#include "../Base/ClientProxy.hpp"

void CmdExecuter::ExecutePassCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server)
{

    if(cmdEntry.args.size() < 1)
    {
        server.SendDataToClient(client, IRCResponseCreator::NeedMoreParams("PASS"));
        return;
    }

    if (client.IsRegistered())
    {
        server.SendDataToClient(client, IRCResponseCreator::AlreadyRegistered());
    }

    std::string password = cmdEntry.args[0];


    if(server.IsPasswordCorrect(cmdEntry.args[0]))
    {
        client.SetHasAuthedTo(true);
        IRCLOG("Client with IP " + client.GetIPAddress() + " has been authed");
        server.SendDataToClient(client, ": Password is correct, you are now authed");
    }
    else
    {
        server.SendDataToClient(client, IRCResponseCreator::PasswordMismatch());
    }

    if(client.IsRegistered())
    {
        SendWelcomeMessage(client, server);
    }
}