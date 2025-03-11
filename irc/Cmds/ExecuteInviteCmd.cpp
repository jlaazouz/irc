#include "CmdExecuter.hpp"
#include "../Base/Server.hpp"
#include "../Base/ClientProxy.hpp"

void CmdExecuter::ExecuteInviteCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server)
{   
    if (cmdEntry.args.size() < 2)
    {
        server.SendDataToClient(client, IRCResponseCreator::NeedMoreParams("INVITE"));
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

    ClientProxy* invitedClient = server.GetClientWithNickname(nickname);
    if (invitedClient == nullptr)
    {
        server.SendDataToClient(client, IRCResponseCreator::NoSuchNick(channelName, nickname));
        return;
    }

    if (channel.IsClientInChannel(invitedClient->GetSocketFD()))
    {
        server.SendDataToClient(client, IRCResponseCreator::AlreadyOnChannel(channelName, nickname));
        return;
    }

    channel.RegisterInvite(invitedClient->GetSocketFD());

    std::string inviteMessageResponse = std::string("341") + " " + client.GetNickname() + " " + nickname + " " + channelName + CRLF;
    server.SendDataToClient(client, inviteMessageResponse);

    std::string inviteMessage = ":" + client.GetNickname() + "!" + client.GetUsername() + "@" + client.GetIPAddress() + " INVITE " + nickname + " :" + channelName + CRLF;
    server.SendDataToClient(*invitedClient, inviteMessage);
}
