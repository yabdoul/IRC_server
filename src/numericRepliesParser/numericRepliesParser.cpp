#include "numericRepliesParser.hpp" 
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <cstdlib>  


NumericTemplateParser* NumericTemplateParser::_instance = 0;

bool NumericTemplateParser::loadFile(const std::string& filePath)
{
    std::ifstream file(filePath.c_str());
    if (!file.is_open())
    {
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') 
            continue;

        size_t pos = line.find('=');
        if (pos == std::string::npos)
            continue; 

        std::string keyStr = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        int key = atoi(keyStr.c_str());
        _templates[key] = value;
    }

    file.close();
    return true;
}

std::string NumericTemplateParser::getTemplate(int code) 
{
    std::map<int, std::string>::const_iterator it = _templates.find(code);
    if (it != _templates.end())
        return it->second;
    return ""; 
}
