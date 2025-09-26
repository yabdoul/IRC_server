#include "commandFactory.hpp"  
 

commandFactory::commandFactory(){} ;   

Command* commandFactory::makeCommand(std::string  command )  
{ 
        if(command ==  "JOIN")  
            return new JoinCommand() ;  
        else if(command == "KICK")  
            return new KickCommand() ;  
        else if(command  ==  "INVITE")
            return  new inviteCommand() ;
        else if(command == "PART")
            return new PartCommand() ;
        return NULL ;    
} ;  