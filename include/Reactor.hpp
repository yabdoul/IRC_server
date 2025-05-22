#include <map>   
#include "EventHandler.hpp" 
#include <iostream>       
class Reactor 
{  
    public :  
        static Reactor& getInstance()      ;  
        Reactor(Reactor &copy  )  ;  
        Reactor& operator=(Reactor &copy ) ;      
    //----------->to define params ----------------------------->
        int Run()   ;  
    //two concret classes thts inherit from EventHandler (Accepthandle  /  Clienthandle) note thts fd after connection changes from fd_server to fd_client 
    //client handle should handle  Read and write event inside it !  
    //
        int registre(int  fd ,  EventHandler  * e  )    ;   
        int unregistre(int fd )   ;    
        int Dispatch()  ;     
        void test(){ std::cout<<"single tone"<<std::endl   ;   } 
    private:  
        Reactor(){} ;  
        ~Reactor(){} ; 
        std::map<int , EventHandler *   >  registred   ;       
}   ;  
