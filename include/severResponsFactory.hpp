#include "Client.hpp"  
#include <map>   
#pragma once 
typedef std::string (*ReplyFunc)(const Client&, const std::string&);    
class serverResponseFactory { 
     public:  
        serverResponseFactory(){} ; 
        ~serverResponseFactory(){} ;  
        static void  respond( int code ,Client &cl    )    ;     
        static std::string replacePlaceholders(const std::string& tmpl,const std::map<std::string, std::string>& values) ;    
    }  ;    