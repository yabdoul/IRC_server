#include "ModeCommand.hpp"   

void ModeCommand::exeChannel(Client &cl, Channel *ch, std::map<std::string, std::string> &params)
{  
    std::cout<<"00000000000000000000000entred Mode execute"<<std::endl  ;   
    if (!cl.getChannel(ch->getName()))
    {
        cl.addMsg(serverResponseFactory::getResp(442, cl, params));
        return;
    }

    if (params.count("mode") == 0)
    {
        cl.addMsg(serverResponseFactory::getResp(324, cl, params));
        return;
    }
    if (!ch->isOp(cl))
    {
        cl.addMsg(serverResponseFactory::getResp(482, cl, params));      
        std::cout<<"sent"<<serverResponseFactory::getResp(482 ,  cl , params) <<std::endl ;   
    }

    std::string mode = params["mode"];
    std::string modeParam = "";
    if (params.find("mode_param") != params.end())
        modeParam = params["mode_param"];

    if (mode.empty())
    {
        cl.addMsg(serverResponseFactory::getResp(461, cl, params));
        return;
    }

    try
    {
        ch->setMode(cl, mode, modeParam);
    }
    catch (const std::exception &e)
    {
        std::cerr << "MODE exception: " << e.what() << std::endl;
        cl.addMsg(serverResponseFactory::getResp(501, cl, params)); 
    }
}

