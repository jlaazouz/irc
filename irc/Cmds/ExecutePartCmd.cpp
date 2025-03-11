#include "CmdExecuter.hpp"
#include "../Base/Server.hpp"
#include "../Base/ClientProxy.hpp"

void CmdExecuter::ExecutePartCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server)
{
    if (cmdEntry.args.empty())
    {
        server.SendDataToClient(client, IRCResponseCreator::NeedMoreParams("PART"));
        return;
    }

    std::vector<std::string> channels = SplitString(cmdEntry.args[0], ',');

    for (size_t i = 0; i < channels.size(); ++i)
    {
        std::string channelName = channels[i];
        if (channelName.empty() || channelName[0] != '#' || !server.IsChannelExist(channelName))
        {
            server.SendDataToClient(client, IRCResponseCreator::NoSuchChannel(channelName));
            continue;
        }


        Channel& channel = server.GetOrCreateChannel(channelName);

        if (!channel.IsClientInChannel(client.GetSocketFD()))
        {
            server.SendDataToClient(client, IRCResponseCreator::NotOnChannel(client.GetNickname(), channelName));
            continue;
        }

        channel.RemoveClient(client.GetSocketFD());
        channel.RemoveOperator(client.GetSocketFD());



        std::string partMessage = ":" + client.GetNickname() + "!" + client.GetUsername() + "@" + client.GetIPAddress() + " PART " + channelName + CRLF;
        CmdExecuter::EchoMessageToAllClientsInChannel(partMessage, client, server, channel, true);

        if (channel.GetClientCount() == 0)
        {
            server.RemoveChannel(channelName);
        }
    }
}
