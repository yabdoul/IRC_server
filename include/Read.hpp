#include "IOstream.hpp"  

class Read : public IOstream    {  
        public : 
            Read() ;   
            ~Read() ;   
            virtual void action(int client_fd ) ;  
        }  ;   