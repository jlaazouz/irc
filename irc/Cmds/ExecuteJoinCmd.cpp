#include "CmdExecuter.hpp"
#include "../Base/Server.hpp"
#include "../Base/ClientProxy.hpp"

void CmdExecuter::ExecuteJoinCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server)
{
    // Check for minimum arguments
    if (cmdEntry.args.empty())
    {
        server.SendDataToClient(client, IRCResponseCreator::NeedMoreParams("JOIN"));
        return;
    }

    // Split the channel names and optional passwords
    std::vector<std::string> channels = SplitString(cmdEntry.args[0], ',');
    std::vector<std::string> passwords;

    if (cmdEntry.args.size() > 1)
    {
        passwords = SplitString(cmdEntry.args[1], ',');
    }

    size_t passwordIndex = -1;

    for (size_t i = 0; i < channels.size(); ++i)
    {
         ++passwordIndex;
        std::string channelName = channels[i];
        if (channelName.empty() || channelName[0] != '#')
        {
            server.SendDataToClient(client, IRCResponseCreator::NoSuchChannel(channelName));
            continue;
        }

        Channel& channel = server.GetOrCreateChannel(channelName);


        if(channel.GetName().empty())
        {
            channel.SetCreationTimeStamp(std::time(NULL));
            //this means it is a new channel
            channel.SetName(channelName);
            //Wich means the client is the first client in the channel, thus he is an operator
            channel.AddOperator(client.GetSocketFD());
        }


        if (channel.IsClientInChannel(client.GetSocketFD()))
            return;

        if (channel.GetLimit() > 0 && channel.GetClientCount() >= channel.GetLimit())
        {
            server.SendDataToClient(client, IRCResponseCreator::ChannelIsFull(channelName));
            continue;
        }
        std::string password = "";
        if (channel.IsPasswordProtected())
        {
            if (passwordIndex < passwords.size())
            {
                password = passwords[passwordIndex];
            }           

            if (password != channel.GetPassword())
            {
                server.SendDataToClient(client, IRCResponseCreator::ChannelIsPasswordProtected(channelName));
                continue;
            }
        }

        if (channel.IsInviteOnly() && !channel.IsClientInvited(client.GetSocketFD()))
        {
            server.SendDataToClient(client, IRCResponseCreator::InviteOnlyChannel(channelName));
            continue;
        }
        else if (channel.IsInviteOnly())
        {
            channel.UnregisterInvite(client.GetSocketFD());
        }

        {
            channel.AddClient(client.GetSocketFD());
        }

        {
            /*
            Prepare the JOIN message that this user will see (the name of the channel plus the nicknames of all the clients in the channel).
            Example: ":nick!user@host JOIN #channel"
                      ": 332 nick #channel : Topic of the channel (if it exists)"
                      ": 353 nick = #channel :@admin nick1 nick2 nick3"
                      ": 366 nick #channel :End of /NAMES list"
            */

            
            std::string joinMessage = IRCResponseCreator::JoinMessage(client.GetNickname(), client.GetUsername(), client.GetIPAddress(), channelName);
            
            std::string topicMessage = "";
            if (!channel.GetTopicName().empty()) { 
                topicMessage = IRCResponseCreator::Topic(channelName, client.GetNickname(), channel.GetTopicName());
            }

            std::string names = "";

            std::vector<int> clientFDs = channel.GetClientsList();
            for (size_t j = 0; j < clientFDs.size(); ++j) {
                ClientProxy* channelClient = server.GetClientWithFD(clientFDs[j]);
                std::string prefix = "";
                
                if (channel.IsOperator(channelClient->GetSocketFD())) {
                    prefix = "@";
                } 

                names += prefix + channelClient->GetNickname();
                
                if (j < clientFDs.size() - 1) {
                    names += " ";
                }
            }

            names += CRLF;

            std::string namesMessage = IRCResponseCreator::NamReply(channelName, client.GetNickname(), names);

            // 4. Send RPL_ENDOFNAMES (366)
            std::string endOfNamesMessage =  IRCResponseCreator::EndOfNames(channelName, client.GetNickname());

             std::string message;
            if(!topicMessage.empty())
               message = joinMessage + topicMessage + namesMessage + endOfNamesMessage;
            else
                message = joinMessage + namesMessage + endOfNamesMessage;


            server.SendDataToClient(client, message);
        }

        {
            //Notify all the clients in the channel that a new client has joined
            std::string joinMessage = IRCResponseCreator::JoinMessage(client.GetNickname(), client.GetUsername(), client.GetIPAddress(), channelName);
            EchoMessageToAllClientsInChannel(joinMessage, client, server, channel, false);
        }
        
    }
}