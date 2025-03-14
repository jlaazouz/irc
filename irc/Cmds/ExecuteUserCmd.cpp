#include "CmdExecuter.hpp"
#include "../Base/Server.hpp"
#include "../Base/ClientProxy.hpp"

void CmdExecuter::ExecuteUserCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server)
{
    // make sure the pass cmd has at least 4 params
    if(cmdEntry.args.size() < 4)
    {
        server.SendDataToClient(client, IRCResponseCreator::NeedMoreParams("USER"));
        return;
    }

    // make sure the password and nickname are set before accepting USER command
    if (!client.GetHasAuthed() || client.GetNickname().empty())
    {
        server.SendDataToClient(client, IRCResponseCreator::NotRegistered());
        return;
    }

    if(client.GetUsername().empty())
    {
        client.SetUsername(cmdEntry.args[0]);
        client.SetHostname(cmdEntry.args[1]);
        client.SetServername(cmdEntry.args[2]);
        client.SetRealname(cmdEntry.args[3]);
    }

    if(client.IsUserRegistrationComplete())
    {
        SendWelcomeMessage(client, server);
    }
}
