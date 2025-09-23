#pragma once  
#include "Client.hpp" 

class Command { 
        public :  
            virtual   std::vector<int>   execute(Client & sender ,  std::map<std::string ,  std::string  >&params  )   =0  ;   
            Command(){} ;   
}  ;     
  

