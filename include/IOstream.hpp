// #include "IOstream.hpp"  

class IOstream    {   
    public : 
        IOstream() ;   
        ~IOstream() ;  
        virtual void action(int client_fd  )=0 ;   

    }   ;  