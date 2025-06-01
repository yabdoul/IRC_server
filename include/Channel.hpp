#include <map>  
#include  "Client.hpp" 
#include "Command.hpp"  
class Channel {  
        private :  
        std::map <std::string , Client>  SubscribedClients ;    
        public :   
        Channel()  {} ; 
        ~Channel() {}   ;      
        void IsSubscribed(Client Client  )  ;   
        void ExecuteCommand(Command  *  cmd  , Client Client  )  ;    
        
} ;   