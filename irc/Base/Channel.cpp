#include "Channel.hpp"

#define HANDLEMODE(_x, x, c) \
    if (_x == x)\
        return;\
    _x = x;\
    if (x == true)\
        _modes.push_back(c);\
    else\
    {\
        std::vector<char>::iterator it = std::find(_modes.begin(), _modes.end(), c);\
        if (it != _modes.end())\
            _modes.erase(it);\
    }\

Channel::Channel(const std::string& name)
{
    _name = name;
    _inviteOnly = false;
    _topicRestricted = false;
    _passwordProtected = false;
    _limit = 0;
}

Channel::Channel()
{
    _inviteOnly = false;
    _topicRestricted = false;
    _passwordProtected = false;
    _limit = 0;
}

Channel::~Channel()
{
}

const std::string& Channel::GetName() const
{
    return _name;
}

void Channel::SetName(const std::string& name)
{
    _name = name;
}

void Channel::AddClient(int clientSocketFD)
{
    _clients.push_back(clientSocketFD);
}

void Channel::RemoveClient(int clientSocketFD)
{
    for(size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i] == clientSocketFD)
        {
            _clients.erase(_clients.begin() + i);
            break;
        }
    }
}

void Channel::AddOperator(int clientSocketFD)
{
    _operators.push_back(clientSocketFD);
}

void Channel::RemoveOperator(int clientSocketFD)
{
    for(size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i] == clientSocketFD)
        {
            _operators.erase(_operators.begin() + i);
            break;
        }
    }
}

bool Channel::IsClientInChannel(int clientSocketFD) const
{
    for(size_t i = 0; i < _clients.size(); i++)
    {
        if (_clients[i] == clientSocketFD)
        {
            return true;
        }
    }
    return false;
}

bool Channel::IsOperator(int clientSocketFD) const
{
    for(size_t i = 0; i < _operators.size(); i++)
    {
        if (_operators[i] == clientSocketFD)
        {
            return true;
        }
    }
    return false;
}

void Channel::SetInviteOnly(bool inviteOnly)
{
    HANDLEMODE(_inviteOnly, inviteOnly, 'i');
}

bool Channel::IsInviteOnly() const
{
    return _inviteOnly;
}

void Channel::SetTopicRestricted(bool topicRestricted)
{
    HANDLEMODE(_topicRestricted, topicRestricted, 't');
}

bool Channel::IsTopicRestricted() const
{
    return _topicRestricted;
}

void Channel::SetPasswordProtected(bool passwordProtected)
{
    HANDLEMODE(_passwordProtected, passwordProtected, 'k');
}

bool Channel::IsPasswordProtected() const
{
    return _passwordProtected;
}

void Channel::SetLimit(int limit)
{
    HANDLEMODE(_limit, limit, 'l');
}

int Channel::GetLimit() const
{
    return _limit;
}

void Channel::SetPassword(const std::string& password)
{
    _password = password;
}

const std::string& Channel::GetPassword() const
{
    return _password;
}

void Channel::SetCreatedAt(const std::string& createdAt)
{
    _createdAt = createdAt;
}

const std::string& Channel::GetCreatedAt() const
{
    return _createdAt;
}

void Channel::SetTopicName(const std::string& topicName)
{
    _topicName = topicName;
}

const std::string& Channel::GetTopicName() const
{
    return _topicName;
}

int Channel::GetClientCount() const
{
    return _clients.size();
}

int Channel::GetOperatorCount() const
{
    return _operators.size();
}


void Channel::SetCreationTimeStamp(long creationTime)
{
    _creationTimeStamp = creationTime;
}
long Channel::GetCreationTimeStamp () const
{
    return _creationTimeStamp;
}

void Channel::RegisterInvite(int clientSocketFD)
{
    _invitedClients.push_back(clientSocketFD);
}

void Channel::UnregisterInvite(int clientSocketFD)
{
    for(size_t i = 0; i < _invitedClients.size(); i++)
    {
        if (_invitedClients[i] == clientSocketFD)
        {
            _invitedClients.erase(_invitedClients.begin() + i);
            break;
        }
    }
}

bool Channel::IsClientInvited(int clientSocketFD) const
{
    for(size_t i = 0; i < _invitedClients.size(); i++)
    {
        if (_invitedClients[i] == clientSocketFD)
        {
            return true;
        }
    }
    return false;
}

const std::vector<int>& Channel::GetClientsList() const
{
    return _clients;
}


std::string Channel::GetModes()
{
    std::string modes = "";
    if (_modes.empty())
        return modes;

    modes += " +" + std::string(_modes.begin(), _modes.end()) ;
    return modes;
}

