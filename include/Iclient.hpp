#include <string>  

class Iclient{  
      public :  
        Iclient() {} ;  
        virtual ~Iclient(){} ;   
        virtual void rcvMsg(std::string&  Msg)  const  =0 ;    

    } ;   