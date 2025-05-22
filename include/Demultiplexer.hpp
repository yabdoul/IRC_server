class Demultiplexer 
{    
    public  :  
        static Demultiplexer& getInstance()  ;  
        Demultiplexer(Demultiplexer &copy)  ;  
        Demultiplexer& operator=(Demultiplexer & copy )    ;     
    private  :  
        Demultiplexer(){} ;  
        ~Demultiplexer(){} ;      

} ;     