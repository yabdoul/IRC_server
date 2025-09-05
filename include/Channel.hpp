#include <map>  
#include  "Client.hpp" 
#include "Command.hpp"    
#include <algorithm>  
#include "Ichannel.hpp"  
#include <algorithm>  
#include <exception>        

class channelException:  public std::exception { 
        private :  
                std::string _reason ;     
        public :
                channelException(std::string reason  )  { 
                        _reason = "[CHANNEL EXCPETION]" + reason ;   
                }  ;     
                ~channelException(){} ;    
                const char*  what() const  noexcept {  
                        return _reason.c_str() ;  
                } ;   

}  ;  
class Channel :  public Ichannel {  
        private :  
                 std::map<Client  , std::string   >    _inviteList  ;    
                 std::vector<Client> _online    ;       
        public :   
                Channel();   
                ~Channel();         
                Channel& operator=(Channel &c )    ;   
                void ExecuteCommand(Command  *  cmd  , Client Client  )  ;    
                void handleKick(Client& sender,  std::string& targetNick, const std::string& reason)  ;   
                void handleJoin(Client& sender )  ;        
                void rcvMsg(std::string  &msg  ) ;    
                void handleInvite(Client& sender, Client& target)  ;   
                // void handleTopic(Client& sender, const std::string& newTopic = "");
                // void handleMode(Client& sender , const std::string& mode, const std::string& param = "") ; 
                // void IsSubscribed(Client &C  )  const    ;   
} ;   