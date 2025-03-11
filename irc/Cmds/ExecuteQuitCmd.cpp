#include "CmdExecuter.hpp"
#include "../Base/Server.hpp"
#include "../Base/ClientProxy.hpp"

void CmdExecuter::ExecuteQuitCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server)
{
    if(cmdEntry.args.size() < 1)
    {
        server.SendDataToClient(client, IRCResponseCreator::NeedMoreParams("QUIT"));
        return;
    }

    std::string quitMessage = cmdEntry.args[0];
    if(cmdEntry.args.size() > 1)
    {
        for(size_t i = 1; i < cmdEntry.args.size(); i++)
        {
            quitMessage += " " + cmdEntry.args[i];
        }
    }

    std::string quitMessageToSend = ":" + client.GetNickname() + "!" + client.GetUsername() + "@" + client.GetIPAddress() + " QUIT :" + quitMessage + CRLF;
    std::vector<Channel*> clientChannels;
    std::vector<Channel*> channels = server.GetChannels();
    for (size_t i = 0; i < channels.size(); ++i)
    {
        if (server.GetChannels()[i]->IsClientInChannel(client.GetSocketFD()))
        {
            clientChannels.push_back(channels[i]);
        }
    }

    for (size_t i = 0; i < channels.size(); ++i)
         CmdExecuter::EchoMessageToAllClientsInChannel(quitMessageToSend, client, server, *clientChannels[i], false);
        
    
    server.RemoveClient(client);
}
