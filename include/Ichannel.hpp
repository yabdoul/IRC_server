#include "Client.hpp"   
#include <string>  

class Ichannel{  
    public:  
        Ichannel(){} ;  
        ~Ichannel(){} ;     
        virtual void lockChannel(Client & sender ) =0 ;      
        virtual  void handleKick(Client& sender,  std::string& targetNick, const std::string& reason) =0  ;   
        virtual  void rcvMsg(std::string&  msg ) =0 ;         
} ;   