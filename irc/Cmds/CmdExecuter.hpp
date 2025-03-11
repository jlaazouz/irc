#ifndef CMD_EXECUTER_HPP
#define CMD_EXECUTER_HPP

#include "../Base/Common.hpp"

struct CmdEntry;
class ClientProxy;
class Server;
class Channel;

class CmdExecuter
{

    private:

    static void SendWelcomeMessage(ClientProxy& client, Server& server);
    static std::vector<std::string> SplitString(const std::string& str, char delimiter);
    static void EchoMessageToAllClientsInChannel(const std::string& message, ClientProxy& sender, Server& server, Channel& channel, bool includeSender = true);

    static void ExecutePassCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server);
    static void ExecuteNickCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server);
    static void ExecuteUserCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server);
    static void ExecuteQuitCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server);
    static void ExecuteJoinCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server);
    static void ExecutePrivmsgCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server);
    static void ExecuteKickCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server);
    static void ExecuteTopicCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server);
    static void ExecuteModeCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server);
    static void ExecuteInviteCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server);
    static void ExecutePartCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server);
    static void ExecuteNoticeCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server);

    CmdExecuter();
    


    
public:
    static void ExecuteCmd(const CmdEntry& cmdEntry, ClientProxy& client, Server& server);
};

#endif // CMD_EXECUTER_HPP