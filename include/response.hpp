#include <vector>   
#include "severResponsFactory.hpp"
class response
{
private:

public:
    response(std::vector<int>& resps ,  Client&c ) ;    
    void sendResp2Server(std::string resp  ,   Client & cl  ) ;    
    ~response(){};
};


