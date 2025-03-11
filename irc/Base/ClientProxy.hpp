#ifndef CLIENT_H
#define CLIENT_H

#include "Common.hpp"

//Note (Ark) :: I've named it ClientProxy because this is just a representation of a client, since this class does not really do anything!
class ClientProxy
{
private:
    int _socketFD;
    std::string _ipAddress; // Client's IP address

    std::string _username; // Client's username
    std::string _nickname; // Client's nickname

    std::string _realname; // Client's real name
    std::string _hostname; // Client's hostname
    std::string _servername; // Client's server name

    bool _hasAuthed; // Client has authenticated using the PASS command with the correct password

 

public:
    ClientProxy(int socketFD, std::string ipAddress);
    ~ClientProxy();
    void Disconnect();
    int GetSocketFD() const;
    std::string GetIPAddress() const;

    void SetUsername(const std::string& username);
    const std::string& GetUsername() const ;

    void SetNickname(const std::string& nickname);
    const std::string& GetNickname() const;

    void SetRealname(const std::string& realname);

    void SetHostname(const std::string& hostname);

    void SetServername(const std::string& servername);

    bool IsRegistered() const;
    void SetHasAuthedTo(bool hasAuthed);

    
};

#endif // CLIENT_H
