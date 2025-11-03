#ifndef NUMERIC_TEMPLATE_PARSER_HPP
#define NUMERIC_TEMPLATE_PARSER_HPP

#include <string>
#include <map>

class NumericTemplateParser
{
private:
    std::map<int, std::string> _templates;
    NumericTemplateParser() {}
    NumericTemplateParser( NumericTemplateParser&);
    NumericTemplateParser& operator=( NumericTemplateParser&);

public:
    static NumericTemplateParser& getInstance();

    bool loadFile(const std::string& filePath);
    std::string getTemplate(int code) ;
};

#endif
