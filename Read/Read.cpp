#include "Read.hpp"
#include <iostream>

Read::Read()
{
    std::cout << "Read constructor called" << std::endl;
}

Read::~Read()
{
    std::cout << "Read destructor called" << std::endl;
}

void Read::action(int client_fd)
{
    std::cout << "Read action called for client_fd: " << client_fd << std::endl;
    // Simple implementation - could read from client socket in a real implementation
}