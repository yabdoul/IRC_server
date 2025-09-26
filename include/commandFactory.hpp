#include "JoinCommand.hpp"  
#include  "KickCommand.hpp"  
#include "inviteCommand.hpp"
#include "PartCommand.hpp"
#include "TopicCommand.hpp"
#include "PrivmsgCommand.hpp"
#include "QuitCommand.hpp"

class commandFactory {  
    public:
        commandFactory() ;    
        static  Command* makeCommand(std::string  command )  ;             
}  ;      