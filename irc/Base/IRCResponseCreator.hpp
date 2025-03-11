#ifndef IRC_RESPONSE_CREATOR_HPP
#define IRC_RESPONSE_CREATOR_HPP

#include <string>

class IRCResponseCreator
{
public:
    // Connection Replies
    static std::string Welcome(const std::string& nickname);
    static std::string YourHost(const std::string& serverName, const std::string& version);
    static std::string Created(const std::string& date);
    static std::string MyInfo(const std::string& serverName, const std::string& version, const std::string& userModes, const std::string& channelModes);

    // Channel Replies
    static std::string Topic(const std::string& channel, const std::string& nickname, const std::string& topic);
    static std::string NoTopic(const std::string& channel);
    static std::string NamReply(const std::string& channel, const std::string& nickname, const std::string& names);
    static std::string EndOfNames(const std::string& channel, const std::string& nickname);
    static std::string ListStart();
    static std::string List(const std::string& channel, const std::string& topic);
    static std::string ListEnd();
    static std::string JoinMessage(const std::string& nickname, const std::string& username, const std::string& ip, const std::string& channel);

    // Error Replies
    static std::string NoSuchNick(const std::string& channel, const std::string& nickname);
    static std::string NoSuchServer(const std::string& serverName);
    static std::string NoSuchChannel(const std::string& channelName);
    static std::string CannotSendToChannel(const std::string& channelName);
    static std::string TooManyChannels(const std::string& channelName);
    static std::string TooManyTargets(const std::string& target);
    static std::string NoRecipient(const std::string& command);
    static std::string NoTextToSend();
    static std::string UnknownCommand(const std::string& command);
    static std::string NoMotd();
    static std::string NoAdminInfo();
    static std::string NoNicknameGiven();
    static std::string ErroneousNickname(const std::string& nickname);
    static std::string NicknameInUse(const std::string& nickname);
    static std::string NickCollision(const std::string& nickname);
    static std::string TheyAreNotInChannel(const std::string& nickname, const std::string& channel);
    static std::string NotOnChannel(const std::string& nickname, const std::string& channel);
    static std::string NotRegistered();
    static std::string NeedMoreParams(const std::string& command);
    static std::string AlreadyRegistered();
    static std::string AlreadyOnChannel(const std::string& channel, const std::string& nickname);
    static std::string PasswordMismatch();
    static std::string ChannelIsFull(const std::string& channel);
    static std::string InviteOnlyChannel(const std::string& channel);
    static std::string BannedFromChannel(const std::string& channel);
    static std::string NoPrivileges();
    static std::string ChanOpPrivilegesNeeded(const std::string& channel);
    static std::string UModeUnknownFlag();
    static std::string UsersDontMatch();
    static ::std::string ChannelIsPasswordProtected(const std::string& channel);

};
#endif // #IRC_RESPONSE_CREATOR_HPP