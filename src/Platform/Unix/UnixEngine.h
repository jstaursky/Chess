#pragma once

#include <ChessEngine/Engine.h>

#include <stdlib.h>
#include <unistd.h>

class UnixEngine : public Engine
{
public:
    UnixEngine (const std::string &path);

    ~UnixEngine() override {};

    void Send (const std::string &message) override;
    bool Receive (std::string &message) override;
private:
    int pipe_to_stockfish[2];
    int pipe_from_stockfish[2];
    pid_t pid;
};
