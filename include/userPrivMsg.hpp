#include "Command.hpp"   

class userPrivMsg : public Command   
{
private:  
public: 
    void execute(Client& sender, std::map<std::string, std::string>& params)  ;  
    userPrivMsg(/* args */){};   
    ~userPrivMsg(){} ; 
};

