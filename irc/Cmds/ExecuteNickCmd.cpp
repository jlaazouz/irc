#include "CmdExecuter.hpp"
#include "../Base/Server.hpp"
#include "../Base/ClientProxy.hpp"

void CmdExecuter::ExecuteNickCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server)
{
    if(cmdEntry.args.size() < 1)
    {
        server.SendDataToClient(client,  IRCResponseCreator::NoNicknameGiven());
        return;
    }
    
    else if (server.IsNicknameInUse(cmdEntry.args[0]))
    {
        server.SendDataToClient(client,  IRCResponseCreator::NicknameInUse(cmdEntry.args[0]));
        return;
    }

    client.SetNickname(cmdEntry.args[0]);

    if(client.IsRegistered())
    {
        SendWelcomeMessage(client, server);
    }
}