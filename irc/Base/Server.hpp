#ifndef SERVER_H
#define SERVER_H

#include "Common.hpp"
#include "ClientProxy.hpp"
#include "Channel.hpp"
#include "ClientsReceiveDataBuffers.hpp"
#include "IRCResponseCreator.hpp"

struct ServerCreateInfo
{
    int Port;
    std::string Password;
};

struct CmdEntry
{
    std::string cmd;
    std::vector<std::string> args;
};


class Server
{
private:
    int _port;
    int _socketFD ;
    std::string _password ;
    std::vector<ClientProxy> _clients;
    std::map<std::string, Channel> _channels; // channels are identified by their names

    static bool s_running;
    void ExecuteMainLoop();
    void HandleReceivedMessage(std::string message, ClientProxy& client);
    void AcceptNewClient();
    void ReceiveData(ClientProxy& client);
    void RemoveClientWithFD(int fd);
    void ProccessLine(const std::string& line, ClientProxy& client);
    CmdEntry ParseCommand(const std::string& line);
    std::vector<pollfd> GetPollfds();
    static  void SignalHandler(int signal);
    ClientsReceiveDataBuffers _clientsReceiveDataBuffers;


public:
    void SendDataToClient(ClientProxy& client, const std::string& data);
    void RemoveClient(ClientProxy& client);
    bool IsPasswordCorrect(const std::string& password) const;
    bool IsNicknameInUse(const std::string& nickname) const;


    void RemoveChannel(const std::string& channelName);
    Channel& GetOrCreateChannel(const std::string& channelName);
    bool IsChannelExist(const std::string& channelName);
    std::vector<Channel*> GetChannels();
    /*
        *Important note (Ark): I return a pointer not a reference because if the client is not found i will return nullptr, with a reference i have to throw an exception or something nasty!
        *Of course the caller should be careful with the returned pointer, or something will explode!
    */
    ClientProxy* GetClientWithFD(int fd);
    ClientProxy* GetClientWithNickname(const std::string& nickname);
    Server(ServerCreateInfo& createInfo);
    void Run();
    ~Server();

};

#endif // SERVER_H