#include "CmdExecuter.hpp"
#include "../Base/Server.hpp"
#include "../Base/ClientProxy.hpp"

void CmdExecuter::ExecuteTopicCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server)
{ 
    //Topic command is used to set the topic of a channel, if the channel is topic restricted only operators can set the topic
    //Topic alone with no arguments will return the current topic of the channel
    if (cmdEntry.args.size() < 1)
    {
        server.SendDataToClient(client, IRCResponseCreator::NeedMoreParams("TOPIC"));
        return;
    }

    std::string channelName = cmdEntry.args[0];
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

    if (cmdEntry.args.size() == 1)
    {
        if (channel.GetTopicName().empty())
        {
            server.SendDataToClient(client, IRCResponseCreator::NoTopic(channelName));
        }
        else
        {
            std::string topicMessage = IRCResponseCreator::Topic(channelName, client.GetNickname(), channel.GetTopicName());
            server.SendDataToClient(client, topicMessage);
        }
    }
    else
    {
        if (!channel.IsOperator(client.GetSocketFD()) && channel.IsTopicRestricted())
        {
            server.SendDataToClient(client, IRCResponseCreator::ChanOpPrivilegesNeeded(channelName));
            return;
        }

        std::string newTopic = cmdEntry.args[1];
        channel.SetTopicName(newTopic);

        std::string topicMessage = IRCResponseCreator::Topic(channelName, client.GetNickname(), newTopic);
        CmdExecuter::EchoMessageToAllClientsInChannel(topicMessage, client, server, channel, true);
    }
}