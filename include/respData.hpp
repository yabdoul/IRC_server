#include "Client.hpp"  
#include "Channel.hpp"  
 
class respData {   
    public:  
        respData()   ;  
        ~respData() ;   
        static std::map<std::string  , std::string>  getRespData(Client &cl  ,     std::map<std::string  , std::string > &params    ) ;    
}  ;   