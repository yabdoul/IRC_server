#include "serverResponse.hpp"   

serverResponse*  serverResponse::getInstance()
{ 
      if(!_instance)  
         _instance =  new serverResponse() ;    
        return _instance ;   
}   ;    
    

void  serverResponse::respond(Client&receiver,int code) 
{   
     /*   
             TODO here goes the logic of responding  to  Client !  
    */
}  ;   