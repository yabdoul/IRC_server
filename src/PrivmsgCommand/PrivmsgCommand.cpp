#include "PrivmsgCommand.hpp"
#include "Server.hpp"
#include "Reactor.hpp"  

PrivmsgCommand::PrivmsgCommand()
{
}

PrivmsgCommand::~PrivmsgCommand()
{
}


 

void PrivmsgCommand:: exeChannel(Client &sender , Channel *ch  , std::map<std::string ,  std::string>&params )
{       
    std::cout<<"Called by"<<sender.getNickName()<<std::endl ;   
    ch->getClients() ;   
    if(!ch)  
    {  
        std::cerr<<"channel Not found "  ; 
        return ;   
    }
    if(!sender.getChannel(ch->getName()))     
    {  
        sender.addMsg(serverResponseFactory::getResp(403 ,  sender , params , ch )) ;     
        return ;     
    }  
    else{   
        //:alice!alice@host PRIVMSG #chatroom :Hello everyone!  
        //:<nick>!<user>@<host> PRIVMSG <#channel> :<message>

        std::string msg = ":" + sender.getNickName() + "!" + sender.getUsername() + "@" + SERVER_NAME + " PRIVMSG #" + params["channel"] + " :" + params["message"];   
        ch->broadcastMessage(sender , msg  , &sender   )  ;           
    }
}