#include "Demultiplexer.hpp"  
#include "Reactor.hpp"  


#define MAX_EVENTS 255

class DemultiplexerException : public std::exception {
   private:
        const std::string _reason;

   public:
        DemultiplexerException(const std::string &reason) : _reason("[DemultiplexerException]: " + reason) {  
           
        }
        ~DemultiplexerException() throw() {}
        virtual char const *what() const throw() {
             return _reason.c_str();
        }
};


Demultiplexer::Demultiplexer() 
{     
   struct epoll_event events[MAX_EVENTS];
   int n = epoll_wait( Reactor::getInstance().getFd()   , events, MAX_EVENTS, -1);
   if (n == -1)
      throw  DemultiplexerException("fatal : epoll_wait()")  ;
   for (int i = 0; i < n; i++) {
      Reactor::getInstance().notify(events[i]);
   }
}