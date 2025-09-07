#include "Client.hpp"  
#include <map>   

typedef std::string (*ReplyFunc)(const Client&, const std::string&);    
class serverResponse { 
    private:  
        serverResponse() {};      
        std::map<int ,  ReplyFunc * >  _responses ;         
        static  serverResponse *  _instance   ;      

        /* 
            [TODO] : Implement function response for each numeric  replies   !  
             
        */     
    public:  
        static serverResponse*  getInstance() ;        
        void  respond( int code ,  std::string &Msg )   ;      
    }  ;    