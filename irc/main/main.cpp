#include "../Base/Server.hpp"


int main(int argc, char** argv)
{

    // the argv must contain the port number and the password
    if(argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    ServerCreateInfo createInfo;
    createInfo.Port = std::stoi(argv[1]);
    createInfo.Password = argv[2];

    try
    {
        Server server(createInfo);
        server.Run();
    }
    catch(const std::exception& e)
    {
        return 1;
    }


    
}