#include "response.hpp"  
#include "numericRepliesParser.hpp"  


void   response::sendResp2Server(std::string  resp ,   Client & cl  )   
{     
       NumericTemplateParser::getInstance()->loadFile("config/numericReplies.txt")  ;   
        /*
             [16-09] Send the response to the Server through  a function (To  be implemented) , and called her on resp 
             Server::getInstance()->sendToClient() ;   
             Not thts rcv function should Be Called in Client Class for encapsulation and repo
        */         
        
}   ;  

response::response(std::vector<int>& resps  , Client &c )  
        { 
            for(std::vector<int>::iterator it  = resps.begin()  ;  it != resps.end() ;  it++ )  
               {    
                    
                   sendResp2Server(serverResponseFactory::replacePlaceholders(NumericTemplateParser::getInstance()->getTemplate(*it)  ,c.userData())  , c ) ;   
                    
               }
        } ;   
