#include "Client.hpp"  
#include <map>   

typedef std::string (*ReplyFunc)(const Client&, const std::string&);    
class serverResponseFactory { 
     public:  
        serverResponseFactory(){} ; 
        ~serverResponseFactory(){} ;  
        void  respond( int code ,  std::string &Msg )   ;     
        std::string replacePlaceholders(const std::string& tmpl,const std::map<std::string, std::string>& values) ;    
        
    }  ;    