#include "CmdExecuter.hpp"
#include "../Base/Server.hpp"
#include "../Base/ClientProxy.hpp"

bool IsAllDigits(const std::string& str) {
    if (str.empty())
        return false;

    for (size_t i = 0; i < str.length(); ++i)
    {
        if (!std::isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

void CmdExecuter::ExecuteModeCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server)
{    
    (void)cmdEntry;
    (void)client;
    (void)server;

    if (cmdEntry.args.empty())
    {
        server.SendDataToClient(client, IRCResponseCreator::NeedMoreParams("MODE"));
        return;
    }

    std::string channelName = cmdEntry.args[0];

    if (channelName.empty() || channelName[0] != '#' || !server.IsChannelExist(channelName))
    {
        server.SendDataToClient(client, IRCResponseCreator::NoSuchChannel(channelName));
        return;
    }
    Channel& channel = server.GetOrCreateChannel(channelName);

    if (cmdEntry.args.size() < 2)
    {
        std::string msg = "324 " + client.GetNickname() + " " + channelName + channel.GetModes() + CRLF;
        std::string timeStamp = "329 " + client.GetNickname() + " " + channelName + " " + std::to_string(channel.GetCreationTimeStamp()) + CRLF;
        msg += timeStamp;

        server.SendDataToClient(client, msg);
    }

    
    // for +/-i or +/-t I must only think about the input until i found the channel mode
    if (cmdEntry.args.size() > 1 && channel.IsOperator(client.GetSocketFD()))
    {
        if (cmdEntry.args[1] == "+i")
        {
            channel.SetInviteOnly(true);
        }
        else if (cmdEntry.args[1] == "-i")
        {
            channel.SetInviteOnly(false);
        }
        else if (cmdEntry.args[1] == "+t")
        {
            channel.SetTopicRestricted(true);
        }
        else if (cmdEntry.args[1] == "-t")
        {
            channel.SetTopicRestricted(false);
        }
        else if (cmdEntry.args[1] == "-l")
        {
            channel.SetLimit(0);
        }
        else if (cmdEntry.args[1] == "+l" && cmdEntry.args.size() > 2)
        {
            if (!IsAllDigits(cmdEntry.args[2]) && cmdEntry.args[2].size() > 4)
            {
                std::string InvalidLimitFormat = "696 " + channelName + " Invalid mode parameter. (l)" + CRLF;
                server.SendDataToClient(client, InvalidLimitFormat);
            }else
            {
                channel.SetLimit(std::stoi(cmdEntry.args[2]));
            }
        }
        else if (cmdEntry.args[1] == "+k" && cmdEntry.args.size() > 2)
        {
            channel.SetPasswordProtected(true);
            channel.SetPassword(cmdEntry.args[2]);
        }
        else if (cmdEntry.args[1] == "-k" && cmdEntry.args.size() > 2)
        {
            channel.SetPasswordProtected(false);
            channel.SetPassword("");
        }
        else if (cmdEntry.args[1] == "+o" && cmdEntry.args.size() > 2)
        {
            ClientProxy *clientChannel = server.GetClientWithNickname(cmdEntry.args[2]);

            if (channel.IsClientInChannel(clientChannel->GetSocketFD()))
            {
                channel.AddOperator(clientChannel->GetSocketFD());
            }
            else
            {
                server.SendDataToClient(client, IRCResponseCreator::NoSuchNick(channelName, cmdEntry.args[2]));
            }
        }
        else if (cmdEntry.args[1] == "-o" && cmdEntry.args.size() > 2)
        {
            ClientProxy *clientChannel = server.GetClientWithNickname(cmdEntry.args[2]);

            if (channel.IsOperator(clientChannel->GetSocketFD()))
            {
                channel.RemoveOperator(clientChannel->GetSocketFD());
            }
            else
            {
                server.SendDataToClient(client, IRCResponseCreator::NoSuchNick(channelName, cmdEntry.args[2]));
            }
        }
    }
}