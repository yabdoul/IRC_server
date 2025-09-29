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
        else if(command == "TOPIC")
            return new TopicCommand() ;
        else if(command == "PRIVMSG")
            return new PrivmsgCommand() ;
        else if(command == "QUIT")
            return new QuitCommand() ;
        else if(command == "PING")
            return new PingCommand() ;
        else if(command == "PONG")
            return new PongCommand() ;  
        else if(command == "NICK")
            return new nickCommand() ;    
        else if(command == "LIST")
            return new ListCommand() ;  
       return NULL ;    
} ;  