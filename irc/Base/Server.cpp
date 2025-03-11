#include "Server.hpp"
#include "../Cmds/CmdExecuter.hpp"

bool Server::s_running = true;

Server::Server(ServerCreateInfo& createInfo)
{
    if(createInfo.Port == -1)
    {
        IRCERROR_AND_CRASH("Invalid port number");
    }

    if (createInfo.Password == "")
    {
        IRCERROR_AND_CRASH("Password cannot be empty");
    }

    _port = createInfo.Port;
    _password = createInfo.Password;
    _socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFD == -1)
    {
        IRCERROR_AND_CRASH("Failed to create socket");
    }

    //set the socket to reuse the address
    int opt = 1;
    if (setsockopt(_socketFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        IRCERROR_AND_CRASH("Failed to set socket to reuse address");
    }

    // Set the socket to non-blocking
    if (fcntl(_socketFD, F_SETFL,  O_NONBLOCK) == -1)
    {
        IRCERROR_AND_CRASH("Failed to set socket to non-blocking");
    }

    // Bind the socket to the port
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);
    if (bind(_socketFD, (sockaddr*)&address, sizeof(address)) == -1)
    {
        IRCERROR_AND_CRASH("Failed to bind socket to port");
    }

    // Listen for incoming connections
    if (listen(_socketFD, SOMAXCONN) == -1)
    {
        IRCERROR_AND_CRASH("Failed to listen for incoming connections");
    }

    IRCINFO("Server started on port " + std::to_string(_port));

    //Handle the signals SIGINT, SIGQUIT AND SIGTERM
    signal(SIGINT, Server::SignalHandler);
    signal(SIGQUIT, Server::SignalHandler);
    signal(SIGTERM, Server::SignalHandler);
}

std::vector<pollfd> Server::GetPollfds()
{
    std::vector<pollfd> pollfds;
    pollfds.reserve(_clients.size() + 1); // +1 for the server socket

    pollfd serverPollfd;
    serverPollfd.fd = _socketFD;
    serverPollfd.events = POLLIN;
    serverPollfd.revents = 0;

    pollfds.push_back(serverPollfd);

    for(size_t i = 0; i < _clients.size(); i++)
    {
        pollfd clientPollfd;
        clientPollfd.fd = _clients[i].GetSocketFD();
        clientPollfd.events = POLLIN;
        clientPollfd.revents = 0;

        pollfds.push_back(clientPollfd);
    }

    return pollfds;
}

void Server::AcceptNewClient()
{
    sockaddr_in clientAddress;
    std::memset(&clientAddress, 0, sizeof(clientAddress));
    socklen_t clientAddressSize = sizeof(clientAddress);
    int clientSocketFD = accept(_socketFD, (sockaddr*)&clientAddress, &clientAddressSize);
    if (clientSocketFD == -1)
    {
        IRCERROR("Failed to accept new client");
        return;
    }

    // Set the client socket to non-blocking
    if (fcntl(clientSocketFD, F_SETFL, O_NONBLOCK) == -1)
    {
        IRCERROR_AND_CRASH("Failed to set client socket to non-blocking");
    }
    std::string clientIPAddress = inet_ntoa(clientAddress.sin_addr);
    _clients.push_back(ClientProxy(clientSocketFD, clientIPAddress));
    IRCINFO("Client " + std::to_string(clientSocketFD) + " Connected");

}

void Server::RemoveChannel(const std::string& channelName)
{
    if (_channels.find(channelName) == _channels.end())
    {
        return;
    }
    _channels.erase(channelName);
}

std::vector<Channel*> Server::GetChannels()
{
    std::vector<Channel*> channels;
    for(std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
    {
        channels.push_back(&it->second);
    }
    return channels;
}

void Server::RemoveClientWithFD(int fd)
{
    for(size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i].GetSocketFD() == fd)
        {
            IRCWARN("Client " + std::to_string(_clients[i].GetSocketFD()) + " Disconnected");
            //Remove the client from all channels
            for(std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
            {
                it->second.RemoveClient(fd);
                it->second.RemoveOperator(fd);
                it->second.UnregisterInvite(fd);
            }
            _clients[i].Disconnect();
            _clients.erase(_clients.begin() + i);
            break;
        }
    }
}

CmdEntry Server::ParseCommand(const std::string& line)
{
    CmdEntry entry;
    std::string trimmedLine = line;
    
    // Trim leading whitespace
    size_t startPos = trimmedLine.find_first_not_of(" \t");
    if (startPos != std::string::npos) {
        trimmedLine = trimmedLine.substr(startPos);
    } else {
        // Line is all whitespace
        return entry;
    }
    
    // Check if line starts with a command prefix (often ':' in IRC)
    if (trimmedLine[0] == ':') {
        // Skip the prefix for now
        size_t prefixEnd = trimmedLine.find(' ');
        if (prefixEnd != std::string::npos) {
            trimmedLine = trimmedLine.substr(prefixEnd + 1);
        } else {
            // Only a prefix, no command
            return entry;
        }
    }
    
    // Extract the command (first word)
    size_t cmdEnd = trimmedLine.find(' ');
    if (cmdEnd != std::string::npos) {
        entry.cmd = trimmedLine.substr(0, cmdEnd);
        trimmedLine = trimmedLine.substr(cmdEnd + 1);
    } else {
        // The entire line is just the command
        entry.cmd = trimmedLine;
        return entry;
    }
    
    // Parse arguments
    size_t pos = 0;
    std::string arg;
    bool inLastParam = false;
    
    while (pos < trimmedLine.length()) {
        // Check if we've hit the last parameter marker (often ':' in IRC)
        if (trimmedLine[pos] == ':' && !inLastParam) {
            // The rest of the line is a single parameter
            entry.args.push_back(trimmedLine.substr(pos + 1));
            break;
        }
        
        // Skip whitespace
        if (trimmedLine[pos] == ' ' || trimmedLine[pos] == '\t') {
            pos++;
            continue;
        }
        
        // Extract argument
        size_t argEnd = trimmedLine.find(' ', pos);
        if (argEnd != std::string::npos) {
            arg = trimmedLine.substr(pos, argEnd - pos);
            pos = argEnd + 1;
        } else {
            // Last argument
            arg = trimmedLine.substr(pos);
            pos = trimmedLine.length();
        }
        
        entry.args.push_back(arg);
    }
    
    return entry;
}

void Server::SendDataToClient(ClientProxy& client, const std::string& data)
{
    int fd = client.GetSocketFD();
    //if the data does not end with \n, add it
    std::string dataToSend = data;
    if (dataToSend.find("\n") == std::string::npos)
    {
        dataToSend += "\n";
    }
    if (send(fd, dataToSend.c_str(), dataToSend.length(), 0) == -1)
    {
        IRCWARN("Failed to send data to client " + std::to_string(fd));
    }
}

void Server::RemoveClient(ClientProxy& client)
{
    int fd = client.GetSocketFD();
    RemoveClientWithFD(fd);

    //Important Note (Ark): The client object is destroyed here, the reference to the client object is invalid after this point
}

void Server::ProccessLine(const std::string& line, ClientProxy& client)
{
    CmdEntry entry = ParseCommand(line);
    if(entry.cmd.empty())
    {
        return;
    }
    //The Real IRC Begins Here
    CmdExecuter::ExecuteCmd(entry, client, *this);

}

ClientProxy* Server::GetClientWithNickname(const std::string& nickname)
{
    for(size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i].GetNickname() == nickname)
        {
            return &_clients[i];
        }
    }
    return nullptr;
}

void Server::HandleReceivedMessage(std::string message, ClientProxy& client)
{
    int fd = client.GetSocketFD();
    _clientsReceiveDataBuffers.AddData(fd, message);
    std::string data = _clientsReceiveDataBuffers.GetData(fd);

    if (data.find_first_of("\r\n") == std::string::npos)
        return;

    _clientsReceiveDataBuffers.DeleteCompleteLines(fd);

    std::vector<std::string> lines;
    size_t pos = 0;
    size_t prevPos = 0;
    while ((pos = data.find_first_of("\r\n", prevPos)) != std::string::npos)
    {
        std::string line = data.substr(prevPos, pos - prevPos);
        lines.push_back(line);
        prevPos = pos + 1;
    }

    for (size_t i = 0; i < lines.size(); i++)
    {
        ProccessLine(lines[i], client);
    }

}

ClientProxy* Server::GetClientWithFD(int fd)
{
    for(size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i].GetSocketFD() == fd)
        {
            return &_clients[i];
        }
    }
    IRCERROR("Client with FD " + std::to_string(fd) + " not found");
    return  nullptr;
}

void Server::ReceiveData(ClientProxy& client)
{
    char buffer[1024] = {0};
    int fd = client.GetSocketFD();
    int bytesRead = recv(fd, buffer, sizeof(buffer), 0);
    if (bytesRead <= 0)
    {
        RemoveClientWithFD(fd);
    }
    else
    {
        std::string message = buffer;
        IRCLOG("Received message: " + message);
        HandleReceivedMessage(message, client);
    }
}

bool Server::IsPasswordCorrect(const std::string& password) const
{
    return password == _password;
}

bool Server::IsNicknameInUse(const std::string& nickname) const
{
    for(size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i].GetNickname() == nickname)
        {
            return true;
        }
    }
    return false;
}

 Channel& Server::GetOrCreateChannel(const std::string& channelName)
{
    return _channels[channelName];
}

bool Server::IsChannelExist(const std::string& channelName)
{
    return _channels.find(channelName) != _channels.end();
}


void Server::ExecuteMainLoop()
{
    std::vector<pollfd> pollfds = GetPollfds();

    int pollCount = poll(pollfds.data(), pollfds.size(), -1);
    if(s_running == false)
        return;
    if (pollCount == -1)
    {
        IRCERROR("Failed to poll sockets");
    }

    for(size_t i = 0; i < pollfds.size(); i++)
    {
        if (pollfds[i].revents & POLLIN)
        {
            if (pollfds[i].fd == _socketFD)
                AcceptNewClient();
            else
                ReceiveData(*GetClientWithFD(pollfds[i].fd));
        }
    }
}

void Server::Run()
{
    while (s_running)
        ExecuteMainLoop();
}


Server::~Server()
{
    close(_socketFD);
    //Disconnect all clients
    for(size_t i = 0; i < _clients.size(); i++)
    {
        RemoveClient(_clients[i]); 
    }
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////STATIC FUNCTIONS//////////////////////////////////////////////////////////////

void Server::SignalHandler(int signal)
{
    IRCINFO("Received signal " + std::to_string(signal) + ". Shutting down server...");
    s_running = false;
}