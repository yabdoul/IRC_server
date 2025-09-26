#include "JoinCommand.hpp"  
#include  "KickCommand.hpp"  
#include "inviteCommand.hpp"
#include "PartCommand.hpp"
#include "TopicCommand.hpp"
#include "PrivmsgCommand.hpp"
#include "QuitCommand.hpp"
#include "PingCommand.hpp"
#include "nickCommand.hpp"
#include "PongCommand.hpp"
#include "ListCommand.hpp"

class commandFactory {  
    public:
        commandFactory() ;    
        static  Command* makeCommand(std::string  command )  ;             
}  ;      