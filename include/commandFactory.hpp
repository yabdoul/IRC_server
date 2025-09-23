#include "JoinCommand.hpp"  
#include  "KickCommand.hpp"  
#include "inviteCommand.hpp"  

class commandFactory {  
    public:
        commandFactory() ;    
        static  Command* makeCommand(std::string  command )  ;             
}  ;      