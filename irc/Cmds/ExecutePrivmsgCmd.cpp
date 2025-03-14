#include "CmdExecuter.hpp"
#include "../Base/Server.hpp"
#include "../Base/ClientProxy.hpp"

// PRIVMSG format of sending messages
std::string    PrivateMessage(const std::string& receiver, ClientProxy& client, const std::string& message)
{
    std::string msg = ":" + client.GetNickname() + "!~" + client.GetUsername() + "@localhost PRIVMSG " + receiver + " :" + message + CRLF; 
    return (msg);
}

std::vector<std::string> tokenizeMessage(const std::string& message) {
    std::vector<std::string> tokens;
    std::istringstream iss(message);
    std::string token;
    
    // Extract command
    iss >> token;
    tokens.push_back(token);
    
    // Extract target
    if (iss >> token) {
        tokens.push_back(token);
        
        // Extract content (everything after ":")
        std::string content;
        if (std::getline(iss, content, ':') && std::getline(iss, content)) {
            tokens.push_back(":" + content);
        }
    }
    
    return tokens;
}

void relayDCCCommand(Server& server, ClientProxy* sender, const std::string& target, const std::string& content) {
    // Find the target client by nickname
    ClientProxy* targetClient = server.GetClientWithNickname(target);
    
    if (targetClient) {
        std::string senderPrefix = sender->GetNickname() + "!~" + 
                                  sender->GetUsername() + "@localhost";
        
        std::string relayMessage = ":" + senderPrefix + " PRIVMSG " + target + " :" + content + CRLF;

        server.SendDataToClient(*targetClient, relayMessage);
    }
}

void handleDCCRequest(Server& server, ClientProxy* sender, const std::string& target, const std::string& content) {
    // Extract the DCC message - handle CTCP format
    std::string dccMessage = content;
    if (dccMessage[0] == ':') dccMessage = dccMessage.substr(1);
    if (dccMessage[0] == 0x01) dccMessage = dccMessage.substr(1);
    
    size_t endPos = dccMessage.find(0x01);
    if (endPos != std::string::npos) {
        dccMessage = dccMessage.substr(0, endPos);
    }
    
    // Parse DCC command
    std::istringstream iss(dccMessage);
    std::string dcc, command;
    iss >> dcc >> command;
    
    if (dcc == "DCC" && command == "SEND") {
        // Format: DCC SEND filename ip_address port filesize
        std::string filename;
        std::string ipAddressStr;
        std::string portStr;
        std::string filesizeStr;
        
        iss >> filename >> ipAddressStr >> portStr >> filesizeStr;
        
        // Find the target client by nickname
        ClientProxy* targetClient = server.GetClientWithNickname(target);
        
        if (targetClient) {
            // Relay the original DCC request
            std::string senderPrefix = sender->GetNickname() + "!~" + 
                                      sender->GetUsername() + "@localhost";
            
            std::string originalMessage = ":" + senderPrefix + " PRIVMSG " + target + " :" + content + CRLF;
            
            // Send to recipient client
            server.SendDataToClient(*targetClient, originalMessage);
            
            // Log the DCC request
            IRCLOG("Relayed DCC SEND request from " << sender->GetNickname() 
                  << " to " << target << " for file: " << filename);
        }
    }
    else if (dcc == "DCC") {
        if (command == "ACCEPT" || command == "RESUME" || command == "CHAT") {
            // Handle other DCC commands by relaying them
            relayDCCCommand(server, sender, target, content);
        }
    }
}

void CmdExecuter::ExecutePrivmsgCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server)
{   
    // ensure there is recipient 
    if (cmdEntry.args.size() < 1)
    {
        server.SendDataToClient(client, IRCResponseCreator::NoRecipient("PRIVMSG"));
        return;
    }

    // ensure there is a message
    if (cmdEntry.args.size() < 2)
    {
        server.SendDataToClient(client, IRCResponseCreator::NoTextToSend());
        return;
    }

    std::string receiver = cmdEntry.args[0]; // could be a channel or a client
    std::string message = cmdEntry.args[1];

    // Check if this is a DCC or CTCP message
    if (message.size() > 2 && message[0] == 0x01) {
        handleDCCRequest(server, &client, receiver, message);
        return; // Exit after handling DCC to avoid double-sending
    }

    // SEND MSG TO CHANNEL
    if (receiver[0] == '#')
    {
        if (!server.IsChannelExist(receiver))
        {
            server.SendDataToClient(client, IRCResponseCreator::NoSuchChannel(receiver));
            return;
        }
        
        Channel& channel = server.GetOrCreateChannel(receiver);
        if (!channel.IsClientInChannel(client.GetSocketFD()))
        {
            server.SendDataToClient(client, IRCResponseCreator::NoSuchNick(receiver, client.GetNickname()));
            return;
        }
        
        int clientCount = channel.GetClientCount();
        std::vector<int> clientList = channel.GetClientsList();
        for (int i = 0; i < clientCount; i++)
        {
            ClientProxy* ChannelClient = server.GetClientWithFD(clientList[i]);
            if (client.GetSocketFD() == ChannelClient->GetSocketFD())
                continue;
            
            server.SendDataToClient(*ChannelClient, PrivateMessage(receiver, client, message));
        }
        return;
    }

    // SEND MSG TO CLIENT
    ClientProxy* TargetClient = server.GetClientWithNickname(receiver);
    if (!server.IsNicknameInUse(receiver))
    {
        server.SendDataToClient(client, IRCResponseCreator::NoSuchNick("", receiver));
    }
    else
    {
        server.SendDataToClient(*TargetClient, PrivateMessage(receiver, client, message));   
    }
}
