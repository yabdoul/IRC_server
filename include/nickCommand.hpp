#include "Command.hpp"  
#include "channelCommand.hpp"


class nickCommand : public Command  {  
    public : 
            nickCommand() {} ;  
            ~nickCommand(){} ;   
          void execute(Client  &cl ,    std::map<std::string ,  std::string >&params )   ;   
}  ;   