#include <map> 
#include <string> 
   
class middleWare{  
    private:
        middleWare();
        std::map<std::string, std::string> _perms;
        static middleWare* _instance;
    public :
        ~middleWare()  ;      
        void addPerm(std::string cmd ,  std::string perms ) ;  
        bool isExist(std::string &cmd ) const ;    
        bool  checkPerm(std::string cmd  ,std::string role   )  ;      
        static middleWare& getInstance();
    } ;  