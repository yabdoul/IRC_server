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

std::string    serverResponseFactory::getResp(int code  ,   Client & cl  )   
{     
    NumericTemplateParser::getInstance()->loadFile("config/numericReplies.txt");           
    std::string response = replacePlaceholders(NumericTemplateParser::getInstance()->getTemplate(code), cl.userData());      
    return response ;    
}  ;   