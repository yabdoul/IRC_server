#include "inviteCommand.hpp"   


void  inviteCommand::execute() { 
      throw std::runtime_error("[INVITE] : is a Channel Command") ;   
}   
  

void  inviteCommand::exeChannel(Client &cl , Channel &ch  , std::map<std::string ,  std::string >&params = g_emptyMap  )  
{ 
     if(ch.isOp(cl))  
     { 
            try{    
                
                ch.inviteUser(sender ,  target )   ;  
            }  
            catch(std::exception &e)  
            { 
                  
            }
     }  
     throw std::runtime_error("[INVITE] : User is Not an Operator") ;      
} ;   
