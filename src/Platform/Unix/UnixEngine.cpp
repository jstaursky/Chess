#include "UnixEngine.h"

#include <cstring>
#include <sstream>

#define READ  0
#define WRITE 1

std::unique_ptr<Engine> Engine::Create (const std::filesystem::path &path)
{
    return std::make_unique<UnixEngine> (path.string());
}

UnixEngine::UnixEngine (const std::string &path)
{
    // Create pipes
    if (pipe (pipe_to_stockfish) < 0 || pipe (pipe_from_stockfish) < 0) {
        perror ("pipe");
        exit (EXIT_FAILURE);
    }

    // Fork a child process
    pid = fork();
    if (pid < 0) {
        perror ("fork");
        exit (EXIT_FAILURE);
    }


    if (pid == 0) {
        // Child Process

        // Close unused ends of pipes
        close (pipe_to_stockfish[WRITE]);
        close (pipe_from_stockfish[READ]);

        // Redirect standard input and output to the pipes
        dup2 (pipe_to_stockfish[READ], STDIN_FILENO);
        dup2 (pipe_from_stockfish[WRITE], STDOUT_FILENO);

        // Close the duplicate file descriptors
        close (pipe_to_stockfish[READ]);
        close (pipe_from_stockfish[WRITE]);

        // Execute chess engine (e.g. Stockfish)
        execlp (path.c_str(), path.c_str(), NULL);
        // If execlp fails
        perror ("execlp");
        exit (EXIT_FAILURE);
    } else {
        // Parent process

        // Close unused ends of pipes
        close (pipe_to_stockfish[READ]);
        close (pipe_from_stockfish[WRITE]);
    }

}


void UnixEngine::Send (const std::string &message)
{
    write (pipe_to_stockfish[WRITE], message.c_str(), message.size() + 1);
}

// The UCI protocol specifies that each command response is terminated with a
// newline character (\n), and the response to the isready command is terminated
// with the string readyok\n.
bool UnixEngine::Receive (std::string &message)
{
    std::stringstream ss;
    char buffer[256] = {0};
    int nbytes_read;

    char *terminator_pos;

    do {
        nbytes_read = read (pipe_from_stockfish[READ], buffer, sizeof (buffer) - 1);
        if (nbytes_read < 0) {
            perror ("read");
            exit (1);
        }
        buffer[nbytes_read] = '\0';
        ss << buffer;
        terminator_pos = strstr (buffer, "\n");
    } while (terminator_pos == NULL);

    message = ss.str();
}
