#include "Command.hpp"  
#include "channelCommand.hpp" 
#include <map>  
#include "severResponsFactory.hpp"

class JoinCommand :  public Command  , public ChannelCommand   
{   
    private:
        void  execute(Client & sender ,  std::map<std::string ,  std::string  >&params  )  {  
             (void ) sender  ;  
             (void ) params ;   
        }          
    public :  
        JoinCommand() {}; 
        ~JoinCommand() {} ;   
        void  exeChannel(Client  &cl ,  Channel &ch   , std::map<std::string ,  std::string >&params  =  g_emptyMap)    ;   

};  