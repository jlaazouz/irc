#include "ClientsReceiveDataBuffers.hpp"

void ClientsReceiveDataBuffers::AddData(int fd, const std::string& data)
{
    buffers[fd] += data;
}

const std::string& ClientsReceiveDataBuffers::GetData(int fd)
{
    return buffers[fd];
}

void ClientsReceiveDataBuffers::ClearAllData(int fd)
{
    buffers[fd].clear();
}

bool ClientsReceiveDataBuffers::EndsWithNewline(const std::string& data)
{
    if (data.empty()) return false;
    char lastChar = data.back();
    return (lastChar == '\r' || lastChar == '\n');
}

std::vector<std::string> ClientsReceiveDataBuffers::DeleteCompleteLines(int fd)
{
    std::vector<std::string> completeLines;
    std::string& buffer = buffers[fd];

    // Find the last newline character
    size_t lastNewlinePos = std::string::npos;
    for (int i = buffer.length() - 1; i >= 0; --i) {
        if (buffer[i] == '\r' || buffer[i] == '\n') {
            lastNewlinePos = i;
            break;
        }
    }

    if (lastNewlinePos == std::string::npos) {
        // No newline characters found, entire buffer is incomplete
        return completeLines;
    }

    // Extract complete part (including the last newline)
    std::string completePart = buffer.substr(0, lastNewlinePos + 1);

    // Parse complete lines
    size_t pos = 0;
    size_t prevPos = 0;

    while ((pos = completePart.find_first_of("\r\n", prevPos)) != std::string::npos) {
        // Extract the line (without newline character)
        std::string line = completePart.substr(prevPos, pos - prevPos);
        completeLines.push_back(line);

        // Skip consecutive newline characters (e.g., \r\n)
        while (pos < completePart.length() && (completePart[pos] == '\r' || completePart[pos] == '\n')) {
            pos++;
        }

        prevPos = pos;
    }

    // Keep only the incomplete part in the buffer
    if (lastNewlinePos + 1 < buffer.length()) {
        buffer = buffer.substr(lastNewlinePos + 1);
    } else {
        buffer.clear();
    }

    return completeLines;
}

void ClientsReceiveDataBuffers::RemoveClient(int fd)
{
    std::map<int, std::string>::iterator it = buffers.find(fd);
    if(it != buffers.end())
        buffers.erase(it);
}