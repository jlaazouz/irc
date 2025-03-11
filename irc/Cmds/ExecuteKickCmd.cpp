#include "CmdExecuter.hpp"
#include "../Base/Server.hpp"
#include "../Base/ClientProxy.hpp"

void CmdExecuter::ExecuteKickCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server)
{
    if (cmdEntry.args.size() < 2)
    {
        server.SendDataToClient(client, IRCResponseCreator::NeedMoreParams("KICK"));
        return;
    }

    std::string nickname = cmdEntry.args[0];
    std::string channelName = cmdEntry.args[1];

    if (channelName.empty() || channelName[0] != '#' || !server.IsChannelExist(channelName))
    {
        server.SendDataToClient(client, IRCResponseCreator::NoSuchChannel(channelName));
        return;
    }

    Channel& channel = server.GetOrCreateChannel(channelName);

    if (!channel.IsClientInChannel(client.GetSocketFD()))
    {
        server.SendDataToClient(client, IRCResponseCreator::NotOnChannel(client.GetNickname(), channelName));
        return;
    }

    if (!channel.IsClientInChannel(client.GetSocketFD()))
    {
        server.SendDataToClient(client, IRCResponseCreator::NotOnChannel(client.GetNickname(), channelName));
        return;
    }

    if (!channel.IsOperator(client.GetSocketFD()))
    {
        server.SendDataToClient(client, IRCResponseCreator::ChanOpPrivilegesNeeded(channelName));
        return;
    }

    ClientProxy* kickedClient = server.GetClientWithNickname(nickname);
    if (kickedClient == nullptr)
    {
        server.SendDataToClient(client, IRCResponseCreator::NoSuchNick(channelName, nickname));
        return;
    }

    if (!channel.IsClientInChannel(kickedClient->GetSocketFD()))
    {
        server.SendDataToClient(client, IRCResponseCreator::TheyAreNotInChannel(nickname, channelName));
        return;
    }

    channel.RemoveClient(kickedClient->GetSocketFD());
    channel.RemoveOperator(kickedClient->GetSocketFD());

    std::string kickMessage = ":" + client.GetNickname() + "!" + client.GetUsername() + "@" + client.GetIPAddress() + " KICK " + channelName + " " + nickname + " :" + cmdEntry.args[2] + CRLF;
    CmdExecuter::EchoMessageToAllClientsInChannel(kickMessage, client, server, channel, false);
}
