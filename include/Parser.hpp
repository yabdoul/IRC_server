// #pragma once
// #include <string>
// #include <vector>
// #include <map>

// /*
//     Parser Task List (Summary):
//     - Tokenize IRC message into prefix, command, and parameters.
//     - Store parsed data in suitable structures.
//     - Provide accessors for command, parameters, and prefix.
//     - Handle malformed messages gracefully.
//     - (Optional) Ensure thread safety if needed.
// */

// class Parser {
// private:
//     std::string prefix;
//     std::string command;
//     std::vector<std::string> parameters;

//     Parser() {}
//     ~Parser() {}
//     Parser(const Parser&) ;  
//     Parser& operator=(const Parser&)   ;

//     static Parser* instance;

// public:
//     static Parser& getInstance();

//     // Parses the IRC message and updates internal state.
//     void Parse(const std::string& msg);

//     // Returns the parsed command.
//     std::string getCommand() const;

//     // Returns the parsed parameters.
//     std::vector<std::string> getParameters() const;

//     // Returns the prefix if present.
//     std::string getPrefix() const;

//     // (Optional) Clear internal state.
//     void clear();
// };