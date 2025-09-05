#include <string>  

class Parser{ 
    public :  
        Parser() ;   
        ~Parser() ;   
        Parser& operator=(Parser& copy )  {   
            (void  ) copy ;     
            return *this ;        
        };   
        Parser& getInstance() ;       
        static void Parse (std::string msg ) ;    
    } ;   