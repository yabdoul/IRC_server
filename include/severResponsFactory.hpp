#include "Client.hpp"  
#include <map>   
#pragma once 
typedef std::string (*ReplyFunc)(const Client&, const std::string&);    
class serverResponseFactory { 
     public:  
        serverResponseFactory(){} ; 
        ~serverResponseFactory(){} ;  
        static std::string getResp( int code ,Client &cl  ,  std::map<std::string , std::string> & params   )    ;     
        static std::string replacePlaceholders( std::string tmpl, std::map<std::string, std::string> values) ;    
    }  ;    