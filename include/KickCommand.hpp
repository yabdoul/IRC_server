#include "Command.hpp" 
 
class KickCommand :  public  Command  
 {    
    private : 
        std::vector<int> execute(Client & sender ,  std::map<std::string ,  std::string  >&params  )  {  
                (void) sender  , (void ) params ;     
                return std::vector<int>() ;  
        }  
    public : 
        KickCommand()   ;   
        ~KickCommand(){} ;  
        virtual void execute()  {};  
} ;  