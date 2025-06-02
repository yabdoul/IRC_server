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


/**
 * @brief Constructor for the Demultiplexer class.
 * 
 * This constructor initializes the Demultiplexer by waiting for events using epoll.
 * It retrieves the file descriptor from the Reactor singleton instance and waits
 * for events using epoll_wait. If epoll_wait fails, it throws a DemultiplexerException.
 * For each event received, it notifies the Reactor instance to handle the event.
 * 
 * @throws DemultiplexerException If epoll_wait fails.
 */
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