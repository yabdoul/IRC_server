#include <map> 
#include <string> 
   
class middleWare{  
    private:
        middleWare() ;   
        std::map<std::string  ,std::string> _perms  ;  
        middleWare * _instance = NULL  ;    
    public :
        ~middleWare()  ;      
        void addPerm(std::string cmd ,  std::string perms ) ;  
        bool isExist(std::string &cmd ) const ;    
        bool  checkPerm(std::string cmd  ,std::string role   )  ;      
        middleWare& getInstance() ;     
    } ;  