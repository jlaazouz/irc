#ifndef CHANNEL_H
#define CHANNEL_H

#include "Common.hpp"


class Channel
{
private:
   /*
    *Important note (Ark): This is a vector of client socket FDs not the actual client objects, i have done this to not deal with the pointer headache!
    *If you want to get the client object from the socket FD you can use the GetClientWithFD function in the Server class
    */
    std::vector<int> _clients;
    std::vector<int> _invitedClients;
    std::vector<int> _operators;
    std::vector<char> _modes;
    std::string _name;
	std::string _password;
	std::string _createdAt;
	std::string _topicName;

    long _creationTimeStamp;
    bool _inviteOnly; // Channel is invite only, only invited clients can join
    bool _topicRestricted; // Only operators can change the topic
    bool _passwordProtected; // Channel is password protected
    int _limit; // Maximum number of clients that can join the channel, -1 means no limit

public:
    Channel(const std::string& name);
    Channel();
    ~Channel();

    const std::string& GetName() const;
    void SetName(const std::string& name);

    void RegisterInvite(int clientSocketFD);
    void UnregisterInvite(int clientSocketFD);

    bool IsClientInvited(int clientSocketFD) const;


    void AddClient(int clientSocketFD);
    void RemoveClient(int clientSocketFD);

    void AddOperator(int clientSocketFD);
    void RemoveOperator(int clientSocketFD);

    bool IsClientInChannel(int clientSocketFD) const;
    bool IsOperator(int clientSocketFD) const;

    void SetInviteOnly(bool inviteOnly);
    bool IsInviteOnly() const;

    void SetTopicRestricted(bool topicRestricted);
    bool IsTopicRestricted() const;

    void SetPasswordProtected(bool passwordProtected);
    bool IsPasswordProtected() const;

    void SetLimit(int limit);
    int GetLimit() const;

    void SetCreationTimeStamp(long creationTime);
    long GetCreationTimeStamp() const;

    void SetPassword(const std::string& password);
    const std::string& GetPassword() const;

    void SetCreatedAt(const std::string& createdAt);
    const std::string& GetCreatedAt() const;

    void SetTopicName(const std::string& topicName);
    const std::string& GetTopicName() const;

    int GetClientCount() const;
    int GetOperatorCount() const;

    std::string GetModes();

    const std::vector<int>& GetClientsList() const;


};

#endif // CHANNEL_H