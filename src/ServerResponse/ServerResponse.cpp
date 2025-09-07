#include "serverResponse.hpp"   
#include "numericRepliesParser.hpp"  

serverResponse* serverResponse::_instance = NULL ;   
serverResponse*  serverResponse::getInstance()
{ 
      if(!_instance)  
         _instance =  new serverResponse() ;    
        return _instance ;   
}   ;    
    

void  serverResponse::respond(int code ,  std::string &Msg  )  
{     
     (void) code  ;  
     (void) Msg  ;    
     NumericTemplateParser::getInstance()->loadFile("config/numericReplies.txt") ;    
     std::cout<<NumericTemplateParser::getInstance()->getTemplate(code) ;   
     /*   
             TODO here goes the logic of responding  to  Client !  
     */
}  ;   