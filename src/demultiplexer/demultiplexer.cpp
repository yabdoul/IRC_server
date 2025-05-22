#include "Demultiplexer.hpp"  
 

// static Demultiplexer:

Demultiplexer& Demultiplexer::getInstance() 
{  
   static Demultiplexer instance ;  
   return instance  ;   
}      
