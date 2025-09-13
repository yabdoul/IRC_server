#include "middleWare.hpp"   


middleWare::middleWare(){  
     addPerm("JOIN" ,  "Invite") ;
} ;      

middleWare&  middleWare::getInstance()
{ 
     if(!_instance )  
          _instance =  new middleWare() ;   
     return *_instance ;    
} ;  

bool middleWare::isExist(std::string &cmd ) const{  
     return((_perms.count(cmd) != 0) ? true : false ) ;      
}  

void middleWare::addPerm(std::string cmd , std::string perms )
{    
     _perms.insert(std::make_pair(cmd,perms)) ;   
} ;       


bool middleWare::checkPerm(std::string cmd  ,  std::string role  ) 
{ 
     return((isExist(cmd) && _perms[cmd] == role )?(true):(false)) ;  
}  ;     