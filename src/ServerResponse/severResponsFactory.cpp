#include "severResponsFactory.hpp"   
#include "numericRepliesParser.hpp"  
#include <map>   



std::string serverResponseFactory::replacePlaceholders( std::string tmpl, std::map<std::string, std::string> values) 
{ 
    std::string result = tmpl;
    for (std::map<std::string, std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        std::string key = "{" + it->first + "}";
        size_t pos = 0;
        while ((pos = result.find(key, pos)) != std::string::npos)
        {
            result.replace(pos, key.length(), it->second);
            pos += it->second.length();
        }
    }
    return result;
}  

void  serverResponseFactory::respond(int code  ,   Client & cl  )   
{     
    NumericTemplateParser::getInstance()->loadFile("config/numericReplies.txt");           
    std::string response = replacePlaceholders(NumericTemplateParser::getInstance()->getTemplate(code), cl.userData());      
    
    // response ends with \r\n
    if (response.length() >= 2 && response.substr(response.length() - 2) != "\r\n") {
        response += "\r\n";
    }
    
    size_t len = response.length();    //not 0 
    if(send(cl.getClientFd(), response.c_str(), len, 0) == -1)
    { 
        throw std::runtime_error("[Response] : Problem in send mechanism");   
    }
    
    std::cout << "Sent numeric " << code << " to " << cl.getNickname() << ": " << response;
 
    // std::map<std::string , std::string >::iterator  it  = cl.userData().begin()    ;   
    // while(it !=   cl.userData().end()  )  
    // {    
    //      std::cout<<it->first<<"="<<it->second<<std::endl;   
    //      it++ ;  
    // }   
    // if(send(cl)) 

}  ;   