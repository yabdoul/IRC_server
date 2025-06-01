#include "Command.hpp" 
 
class KickCommand :  public  Command  
 { 
    public : 
        KickCommand()   ;   
        ~KickCommand(){} ;  
        virtual void execute()  {};  
} ;  