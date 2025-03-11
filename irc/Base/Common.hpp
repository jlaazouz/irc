#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <cstring>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <map>
#include <sstream>
#include <ctime>


//some colors
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define NORMAL "\033[0m"

#define CRLF "\r\n"


//We will follow IRC Protocol RFC2119


// Define a macro to print an error message and throw an exception
#define IRCERROR_AND_CRASH(msg) \
    do { \
        std::cerr << RED << "FATAL Error: " << msg << NORMAL << std::endl; \
        throw std::runtime_error(msg); \
    } while(0)

#define IRCERROR(msg) std::cerr << RED << "Error: " << msg << NORMAL << std::endl

#define IRCINFO(msg) std::cout << GREEN << "Info: " << msg << NORMAL << std::endl

#define IRCWARN(msg) std::cout << YELLOW << "Warning: " << msg << NORMAL << std::endl

#define IRCLOG(msg) std::cout << msg << std::endl



#endif //COMMON_H
