#include "Command.hpp"  

class userCommand :  public Command  { 
      public :  
        userCommand () {} ;  
        ~userCommand(){} ;     
        void    execute(Client & sender ,  std::map<std::string ,  std::string  >&params  ) ;  
} ;    