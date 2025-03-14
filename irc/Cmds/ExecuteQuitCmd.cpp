#include "CmdExecuter.hpp"
#include "../Base/Server.hpp"
#include "../Base/ClientProxy.hpp"

void CmdExecuter::ExecuteQuitCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server)
{
    // ensure there the user write a message before quitting
    if(cmdEntry.args.size() < 1)
    {
        server.SendDataToClient(client, IRCResponseCreator::NeedMoreParams("QUIT"));
        return;
    }

    std::string quitMessage = cmdEntry.args[0];

    std::string quitMessageToSend = ":" + client.GetNickname() + "!" + client.GetUsername() + "@" + client.GetIPAddress() + " QUIT :" + quitMessage + CRLF;
    std::vector<Channel*> clientChannels;
    std::vector<Channel*> channels = server.GetChannels();

    // Search for all the channels that the client is part of
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
