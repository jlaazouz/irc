#include "CmdExecuter.hpp"
#include "../Base/Server.hpp"
#include "../Base/ClientProxy.hpp"

void CmdExecuter::ExecuteNickCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server)
{
    // make sure the pass cmd has at least one param
    if(cmdEntry.args.size() < 1)
    {
        server.SendDataToClient(client,  IRCResponseCreator::NoNicknameGiven());
        return;
    }

    // check if the user has registered (password provided)
    if (!client.GetHasAuthed())
    {
        server.SendDataToClient(client, IRCResponseCreator::NotRegistered());
        return;
    }

    // Don't accept any more NICK commands
    if (!client.GetNickname().empty())
    {
        server.SendDataToClient(client,  IRCResponseCreator::AlreadyRegistered());
        return;
    }
    
    else if (server.IsNicknameInUse(cmdEntry.args[0]))
    {
        server.SendDataToClient(client,  IRCResponseCreator::NicknameInUse(cmdEntry.args[0]));
        return;
    }

    client.SetNickname(cmdEntry.args[0]);
}