#include "CmdExecuter.hpp"
#include "../Base/Server.hpp"
#include "../Base/ClientProxy.hpp"

void CmdExecuter::ExecuteUserCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server)
{
    if(cmdEntry.args.size() < 4)
    {
        server.SendDataToClient(client, IRCResponseCreator::NeedMoreParams("USER"));
        return;
    }

    if(client.GetUsername().empty())
    {
        client.SetUsername(cmdEntry.args[0]);
        client.SetHostname(cmdEntry.args[1]);
        client.SetServername(cmdEntry.args[2]);
        client.SetRealname(cmdEntry.args[3]);
    }

    if(client.IsRegistered())
    {
        SendWelcomeMessage(client, server);
    }
}
