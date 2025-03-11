#include "IRCResponseCreator.hpp"
#include "Common.hpp"

// Connection Replies
std::string IRCResponseCreator::Welcome(const std::string& nickname)
{
    return "001 " + nickname + " :Welcome to the Internet Relay Network!" + CRLF;
}

std::string IRCResponseCreator::YourHost(const std::string& serverName, const std::string& version)
{
    return "002 :Your host is " + serverName + ", running version " + version + CRLF;
}

std::string IRCResponseCreator::Created(const std::string& date)
{
    return "003 :This server was created " + date;
}

std::string IRCResponseCreator::MyInfo(const std::string& serverName, const std::string& version, const std::string& userModes, const std::string& channelModes)
{
    return "004 " + serverName + " " + version + " " + userModes + " " + channelModes + CRLF;
}

// Channel Replies
std::string IRCResponseCreator::Topic(const std::string& channel, const std::string& nickname, const std::string& topic)
{
    return "332 " + nickname + " " + channel + " :" + topic + CRLF;
}

std::string IRCResponseCreator::NoTopic(const std::string& channel)
{
    return "331 " + channel + " :No topic is set" + CRLF;
}

std::string IRCResponseCreator::NamReply(const std::string& channel, const std::string& nickname, const std::string& names)
{
    return  "353 " + nickname + " = " + channel + " :" + names + CRLF;
}

std::string IRCResponseCreator::EndOfNames(const std::string& channel, const std::string& nickname)
{
    return "366 " + nickname + " " + channel + " :End of /NAMES list" + CRLF;
}

std::string IRCResponseCreator::ListStart()
{
    return std::string("321 Channel :Users  Name") + CRLF;
}

std::string IRCResponseCreator::List(const std::string& channel, const std::string& topic)
{
    return "322 " + channel + " :" + topic + CRLF;
}

std::string IRCResponseCreator::ListEnd()
{
    return std::string("323 :End of /LIST") + CRLF;
}

// Error Replies
std::string IRCResponseCreator::NoSuchNick(const std::string& channel, const std::string& nickname)
{
    return "401 " + channel + " " + nickname + " :No such nick/channel" + CRLF;
}

std::string IRCResponseCreator::NoSuchServer(const std::string& serverName)
{
    return "402 " + serverName + " :No such server";
}

std::string IRCResponseCreator::NoSuchChannel(const std::string& channelName)
{
    return "403 " + channelName + " :No such channel" + CRLF;
}

std::string IRCResponseCreator::CannotSendToChannel(const std::string& channelName)
{
    return "404 " + channelName + " :Cannot send to channel" + CRLF;
}

std::string IRCResponseCreator::TooManyChannels(const std::string& channelName)
{
    return "405 " + channelName + " :You have joined too many channels" + CRLF;
}

std::string IRCResponseCreator::TooManyTargets(const std::string& target)
{
    return "407 " + target + " :Too many recipients" + CRLF;
}

std::string IRCResponseCreator::NoRecipient(const std::string& command)
{
    return "411 " + command + " :No recipient given" + CRLF;
}

std::string IRCResponseCreator::NoTextToSend()
{
    return std::string("412 :No text to send") + CRLF;
}

std::string IRCResponseCreator::UnknownCommand(const std::string& command)
{
    return "421 " + command + " :Unknown command" + CRLF;
}

std::string IRCResponseCreator::NoMotd()
{
    return std::string("422 :MOTD File is missing") + CRLF;
}

std::string IRCResponseCreator::NoAdminInfo()
{
    return std::string("423 :No administrative info available") + CRLF;
}

std::string IRCResponseCreator::NoNicknameGiven()
{
    return std::string("431 :No nickname given") + CRLF;
}

std::string IRCResponseCreator::ErroneousNickname(const std::string& nickname)
{
    return "432 " + nickname + " :Erroneous nickname" + CRLF;
}

std::string IRCResponseCreator::NicknameInUse(const std::string& nickname)
{
    return "433 " + nickname + " :Nickname is already in use" + CRLF;
}

std::string IRCResponseCreator::NickCollision(const std::string& nickname)
{
    return "436 " + nickname + " :Nickname collision KILL" + CRLF;
}

std::string IRCResponseCreator::NotRegistered()
{
    return std::string("451 :You have not registered") + CRLF;
}

std::string IRCResponseCreator::NeedMoreParams(const std::string& command)
{
    return "461 " + command + " :Not enough parameters" + CRLF;
}

std::string IRCResponseCreator::AlreadyRegistered()
{
    return std::string("462 :You may not reregister")  + CRLF;
}

std::string IRCResponseCreator::PasswordMismatch()
{
    return std::string("464 :Password incorrect") + CRLF;
}

std::string IRCResponseCreator::NoPrivileges()
{
    return std::string("481 :Permission Denied- You're not an IRC operator") + CRLF;
}

std::string IRCResponseCreator::ChanOpPrivilegesNeeded(const std::string& channel)
{
    return "482 " + channel + " :You're not channel operator" + CRLF;
}

std::string IRCResponseCreator::ChannelIsFull(const std::string& channel)
{
    return "471 " + channel + " :Cannot join channel (+l)" + CRLF;
}

std::string IRCResponseCreator::InviteOnlyChannel(const std::string& channel)
{
    return "473 " + channel + " :Cannot join channel (+i)" + CRLF;
}

std::string IRCResponseCreator::ChannelIsPasswordProtected(const std::string& channel)
{
    return "475 " + channel + " :Cannot join channel (+k)" + CRLF;
}

std::string IRCResponseCreator::JoinMessage(const std::string& nickname, const std::string& username, const std::string& ip, const std::string& channel)
{
    return ":" + nickname + "!" + username + "@" + ip + " JOIN " + channel + CRLF;
}

std::string IRCResponseCreator::AlreadyOnChannel(const std::string& channel, const std::string& nickname)
{
    return "443 " + nickname + " " + channel + " :is already on channel" + CRLF;
}

std::string IRCResponseCreator::TheyAreNotInChannel(const std::string& nickname, const std::string& channel)
{
    return "441 " + nickname + " " + channel + " :They aren't on that channel" + CRLF;
}


std::string IRCResponseCreator::NotOnChannel(const std::string& nickname, const std::string& channel)
{
    return "442 " + nickname + " " + channel + " :You're not on that channel" + CRLF;
}