#include "severResponsFactory.hpp"   
#include "numericRepliesParser.hpp"  


void  serverResponseFactory::respond(int code ,  std::string &Msg  )  
{     
     (void) code  ;  
     (void) Msg  ;      
     NumericTemplateParser::getInstance()->loadFile("config/numericReplies.txt")  ;         
     std::cout<<NumericTemplateParser::getInstance()->getTemplate(code)   ;   
        
     /*   
             TODO here goes the logic of responding  to  Client !  
     */
}  ;   

std::string serverResponseFactory::replacePlaceholders(const std::string& tmpl,const std::map<std::string, std::string>& values) 
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