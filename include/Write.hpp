#include "IOstream.hpp"

class Write : public IOstream
{
public:
        Write();
        Write(const Write& other);
        Write& operator=(const Write& other);
        ~Write();

        virtual void action(int client_fd ) ;
};