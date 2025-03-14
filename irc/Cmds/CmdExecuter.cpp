#include "CmdExecuter.hpp"
#include "../Base/Server.hpp"
#include "../Base/ClientProxy.hpp"

void CmdExecuter::SendWelcomeMessage(ClientProxy& client, Server& server)
{
    server.SendDataToClient(client, "001 " + client.GetNickname() +  " :Welcome to the Internet Relay Network " + CRLF);
}

void CmdExecuter::EchoMessageToAllClientsInChannel(const std::string& message, ClientProxy& sender, Server& server, Channel& channel, bool includeSender)
{
    std::vector<int> clientFDs = channel.GetClientsList();
    for(size_t i = 0; i < clientFDs.size(); i++)
    {
        if(!includeSender && clientFDs[i] == sender.GetSocketFD())
        {
            continue;
        }

        ClientProxy* channelClient = server.GetClientWithFD(clientFDs[i]);
        server.SendDataToClient(*channelClient, message);
    }
}



// Function to split a string by a delimiter using C++ standard library
std::vector<std::string> CmdExecuter::SplitString(const std::string& str, char delimiter)
{
    std::vector<std::string> result;
    std::string token;
    std::istringstream tokenStream(str);
    
    // Split string by delimiter
    while (std::getline(tokenStream, token, delimiter))
    {
        // Remove any leading/trailing spaces
        token.erase(token.find_last_not_of(" \n\r\t") + 1);
        token.erase(0, token.find_first_not_of(" \n\r\t"));

        
        result.push_back(token);
    }
    return result;
}

void CmdExecuter::ExecuteCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server)
{
    std::string cmd = cmdEntry.cmd;
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

    if(cmd == "PASS")
    {
        ExecutePassCmd(cmdEntry, client, server);
    }
    else if(cmd == "NICK")
    {
        ExecuteNickCmd(cmdEntry, client, server);
    }
    else if(cmd == "USER")
    {
        ExecuteUserCmd(cmdEntry, client, server);
    }
    else if(cmd == "QUIT")
    {
        ExecuteQuitCmd(cmdEntry, client, server);
    }

    //The blow commands require the client to be registered
    else if(!client.IsUserRegistrationComplete())
    {
        server.SendDataToClient(client, std::string("451 You have not registered ") + cmdEntry.cmd + CRLF);
        IRCLOG("Client with IP " + client.GetIPAddress() + " tried to execute a command without registering [" + cmdEntry.cmd + "]");
        return;
    }
    else if(cmd == "JOIN")
    {
        ExecuteJoinCmd(cmdEntry, client, server);
    }
    else if(cmd == "PRIVMSG")
    {
        ExecutePrivmsgCmd(cmdEntry, client, server);
    }
    else if(cmd == "KICK")
    {
        ExecuteKickCmd(cmdEntry, client, server);
    }
    else if(cmd == "TOPIC")
    {
        ExecuteTopicCmd(cmdEntry, client, server);
    }
    else if(cmd == "MODE")
    {
        ExecuteModeCmd(cmdEntry, client, server);
    }
    else if(cmd == "INVITE")
    {
        ExecuteInviteCmd(cmdEntry, client, server);
    }
    else if(cmd == "PART")
    {
        ExecutePartCmd(cmdEntry, client, server);
    }
    else if(cmd == "QUIT")
    {
        ExecuteQuitCmd(cmdEntry, client, server);
    }
    else if(cmd == "NOTIC")
    {
        ExecuteNoticeCmd(cmdEntry, client, server);
    }
    else
    {
        server.SendDataToClient(client, "421 :Unknown command");
    }

}



















