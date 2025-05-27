#include "Write.hpp"
#include <iostream>

Write::Write()
{
    std::cout << "Write constructor called" << std::endl;
}

Write::Write(const Write& other)
{
    std::cout << "Write copy constructor called" << std::endl;
    (void)other; // Unused parameter
}

Write& Write::operator=(const Write& other)
{
    std::cout << "Write assignment operator called" << std::endl;
    if (this != &other)
    {
        // Nothing to copy in this simple implementation
    }
    return *this;
}

Write::~Write()
{
    std::cout << "Write destructor called" << std::endl;
}

void Write::action(int client_fd)
{
    std::cout << "Write action called for client_fd: " << client_fd << std::endl;
    // Simple implementation - could write to client socket in a real implementation
}