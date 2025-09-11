#include "Client.hpp"   
#include <string>  

class Ichannel{  
    public:  
        Ichannel(){} ;  
        ~Ichannel(){} ;     
        virtual void lockChannel(Client & sender ) =0 ;      
        virtual  void handleKick(Client& sender,  std::string& targetNick, const std::string& reason) =0  ;   
        virtual  void handleJoin(Client& sender  )   =  0 ;    
        virtual  void rcvMsg(std::string&  msg ) =0 ;         
    //    virtual  void handleInvite(Client& sender, Client& target) = 0 ;
    //    virtual  void handleTopic(Client& sender, const std::string& newTopic = "")  =0 ;
    //    virtual  void handleMode(Client& sender , const std::string& mode, const std::string& param = "") =0; 
    //    virtual  void IsSubscribed(Client &C  )  const =0  ;   
} ;   