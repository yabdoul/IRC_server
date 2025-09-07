#ifndef NUMERIC_TEMPLATE_PARSER_HPP
#define NUMERIC_TEMPLATE_PARSER_HPP

#include <string>
#include <map>

class NumericTemplateParser
{
private:
    static NumericTemplateParser* _instance;
    std::map<int, std::string> _templates;
    NumericTemplateParser() {}
    NumericTemplateParser(const NumericTemplateParser&);
    NumericTemplateParser& operator=(const NumericTemplateParser&);

public:
    static NumericTemplateParser* getInstance()
    {
        if (!_instance)
            _instance = new NumericTemplateParser();
        return _instance;
    }

    bool loadFile(const std::string& filePath);
    std::string getTemplate(int code) const;
};

#endif
