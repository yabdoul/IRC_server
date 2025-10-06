#include "severResponsFactory.hpp"   
#include "numericRepliesParser.hpp"  
#include "respData.hpp"
#include <map>   



std::string serverResponseFactory::replacePlaceholders(std::string tmpl, std::map<std::string, std::string> values, Channel *c  )
{
    std::string result = tmpl;

    // Handle the special {nicklist} placeholder
    const std::string nicklistKey = "{nicklist}";
    size_t nicklistPos = result.find(nicklistKey);
    if (nicklistPos != std::string::npos)
    {
        std::string nicklist;
        const std::vector<Client*>& users = c->getUsers();
        for (size_t i = 0; i < users.size(); ++i)
        {
            if (i > 0)
                nicklist += " ";
            nicklist += users[i]->getNickname();
        }
        result.replace(nicklistPos, nicklistKey.length(), nicklist);
    }

    // Replace other placeholders
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

std::string    serverResponseFactory::getResp(int code  ,   Client & cl   , std::map<std::string , std::string>& params ,  Channel *c     )   
{     
    NumericTemplateParser::getInstance()->loadFile("config/numericReplies.txt");   
    std::string response   ;    
    if(code ==  1001  )  
        response = replacePlaceholders(":{nick}!{user}@{host} JOIN :#{channel}" ,  respData::getRespData(cl , params) ,  c ) ;        
    else 
        response = replacePlaceholders(NumericTemplateParser::getInstance()->getTemplate(code),    respData::getRespData(cl , params   )  ,c  );      
    return response ;    
}  ;   