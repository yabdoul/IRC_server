#include "Reactor.hpp"  


 Reactor&  Reactor::getInstance()   
{   
     static Reactor Reactor ;  
     return Reactor ;     
}   
 