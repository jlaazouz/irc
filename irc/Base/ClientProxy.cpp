#include "ClientProxy.hpp"


ClientProxy::ClientProxy(int socketFD, std::string ipAddress)
{
    _socketFD = socketFD;
    _ipAddress = ipAddress;

    _username = "";
    _nickname = "";

    _hasAuthed = false;

}


ClientProxy::~ClientProxy()
{
}

int ClientProxy::GetSocketFD() const
{
    return _socketFD;
}

std::string ClientProxy::GetIPAddress() const
{
    return _ipAddress;
}

void ClientProxy::Disconnect()
{
    close(_socketFD);
}

void ClientProxy::SetUsername(const std::string& username)
{
    _username = username;
}

const std::string& ClientProxy::GetUsername() const
{
    return _username;
}

void ClientProxy::SetNickname(const std::string& nickname)
{
    _nickname = nickname;
}

const std::string& ClientProxy::GetNickname() const
{
    return _nickname;
}

bool ClientProxy::IsRegistered() const
{
    return !_username.empty() && !_nickname.empty() && _hasAuthed;
}

void ClientProxy::SetHasAuthedTo(bool hasAuthed)
{
    _hasAuthed = hasAuthed;
}

void ClientProxy::SetRealname(const std::string& realname)
{
    _realname = realname;
}


void ClientProxy::SetHostname(const std::string& hostname)
{
    _hostname = hostname;
}


void ClientProxy::SetServername(const std::string& servername)
{
    _servername = servername;
}