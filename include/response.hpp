#include <vector>   
#include "severResponsFactory.hpp"
class Response
{
private:
    Response* _instance ;   
    Response() ;    
public:
    void sendResp2Server(std::string resp , unsigned int Client_fd )  ;    
    Response& getInstance() 
    {  
          if(!_instance)   
                _instance  = new Response() ;  
          return  *_instance ;    

    }  
    ~Response(){ 
         delete _instance ;  
    }  ;  
    
};


