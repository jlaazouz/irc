#ifndef CLIENTS_RECEIVE_DATA_BUFFERS_HPP
#define CLIENTS_RECEIVE_DATA_BUFFERS_HPP

#include <map>
#include <string>
#include <vector>

class ClientsReceiveDataBuffers
{
private:
    std::map<int /*FD*/, std::string> buffers;

public:
    // Add data to the buffer for a specific file descriptor
    void AddData(int fd, const std::string& data);
    
    // Get the current buffer content for a specific file descriptor
    const std::string& GetData(int fd);
    
    // Clear all data in the buffer for a specific file descriptor
    void ClearAllData(int fd);
    
    // Process and clear complete lines, keep any incomplete part
    // Returns a vector of complete lines processed
    std::vector<std::string> DeleteCompleteLines(int fd);
    
    // Check if the given buffer ends with a newline character
    static bool EndsWithNewline(const std::string& data);
};

#endif // CLIENTS_RECEIVE_DATA_BUFFERS_HPP