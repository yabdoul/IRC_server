# IRC Server

A custom implementation of an Internet Relay Chat (IRC) server written in C++.

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Building](#building)
- [Usage](#usage)
- [Configuration](#configuration)
- [Supported IRC Commands](#supported-irc-commands)
- [Architecture](#architecture)
- [Development](#development)
- [Testing](#testing)
- [Contributing](#contributing)
- [License](#license)

## 🎯 Overview

This IRC server is designed to handle multiple client connections simultaneously, supporting standard IRC protocol operations including user registration, channel management, and message routing. The server implements a modular response system with configurable numeric reply templates.

## ✨ Features

- **Multi-client Support**: Handle multiple simultaneous connections
- **Standard IRC Protocol**: Implements core IRC commands and responses
- **Configurable Responses**: Template-based numeric reply system
- **Channel Management**: Create, join, and manage chat channels
- **User Authentication**: Nickname and user registration system
- **Singleton Design Pattern**: Efficient resource management for core components

## 📁 Project Structure

```
IRC_server/
├── src/
│   └── ServerResponse/
│       ├── ServerResponse.cpp
│       └── severResponsFactory.cpp
├── include/
│   ├── serverResponse.hpp
│   ├── severResponsFactory.hpp
│   └── numericRepliesParser.hpp
├── config/
│   └── numericReplies.txt
├── Makefile
└── README.md
```

## 🔧 Prerequisites

- **Compiler**: GCC or Clang with C++98 support
- **Operating System**: Linux/Unix-based system
- **Make**: Build automation tool

## 🛠️ Building

1. **Clone the repository**:
   ```bash
   git clone <repository-url>
   cd IRC_server
   ```

2. **Compile the project**:
   ```bash
   make
   ```

3. **Clean build files** (optional):
   ```bash
   make clean
   ```

## 🚀 Usage

### Starting the Server

```bash
./ircserver <port> <password>
```

**Parameters**:
- `<port>`: Port number for the server to listen on (e.g., 6667)
- `<password>`: Server password for client authentication

**Example**:
```bash
./ircserver 6667 mypassword
```

### Connecting with IRC Clients

You can connect using any standard IRC client:

```bash
# Using netcat for testing
nc localhost 6667

# Using irssi
irssi -c localhost -p 6667

# Using HexChat, Weechat, or other GUI clients
```

## ⚙️ Configuration

### Numeric Replies Configuration

Edit `config/numericReplies.txt` to customize server responses:

```
001 Welcome to the {network} Network, {nick}!{user}@{host}
002 Your host is {servername}, running version {version}
003 This server was created {datetime}
...
```

**Available Placeholders**:
- `{nick}`: Client nickname
- `{user}`: Username
- `{host}`: Client hostname
- `{channel}`: Channel name
- `{servername}`: Server name
- `{network}`: Network name
- `{version}`: Server version
- `{datetime}`: Current date/time

## 📡 Supported IRC Commands

### User Commands
- `NICK <nickname>` - Set or change nickname
- `USER <username> <hostname> <servername> <realname>` - User registration
- `PASS <password>` - Server password authentication
- `QUIT [message]` - Disconnect from server

### Channel Commands
- `JOIN <channel>` - Join a channel
- `PART <channel> [message]` - Leave a channel
- `PRIVMSG <target> <message>` - Send message to user or channel
- `TOPIC <channel> [topic]` - View or set channel topic

### Server Commands
- `PING <server>` - Ping server
- `PONG <server>` - Pong response

## 🏗️ Architecture

### Core Components

1. **ServerResponse**: Singleton class managing server responses
2. **ServerResponseFactory**: Factory pattern for creating appropriate responses
3. **NumericTemplateParser**: Parses and manages configurable response templates

### Design Patterns Used

- **Singleton Pattern**: For global server state management
- **Factory Pattern**: For response generation
- **Template Method**: For configurable message formatting

### Key Classes

```cpp
class serverResponse {
    static serverResponse* getInstance();
    void respond(Client& receiver, int code);
};

class serverResponseFactory {
    static void respond(int code, std::string& msg);
    static std::string replacePlaceholders(const std::string& tmpl, 
                                         const std::map<std::string, std::string>& values);
};
```

## 🔧 Development

### Adding New Commands

1. Create command handler in appropriate source file
2. Add command parsing logic
3. Update response templates in `config/numericReplies.txt`
4. Test with IRC client

### Code Style Guidelines

- Use consistent spacing and formatting
- Follow C++98 standard
- Document public methods
- Use meaningful variable names
- Implement proper error handling

## 🧪 Testing

### Manual Testing
```bash
# Start server
./ircserver 6667 test123

# Connect with telnet
telnet localhost 6667

# Send IRC commands
PASS test123
NICK testuser
USER testuser 0 * :Test User
JOIN #test
PRIVMSG #test :Hello World!
```

### Automated Testing
```bash
# Run test suite (when implemented)
make test
```

## 🚧 Current Status

**Implemented**:
- ✅ Basic server response framework
- ✅ Template-based numeric replies
- ✅ Placeholder replacement system

**In Progress**:
- 🔄 Socket connection handling
- 🔄 IRC command parsing
- 🔄 Client management

**TODO**:
- ❌ Complete IRC protocol implementation
- ❌ Channel management system
- ❌ User authentication
- ❌ Comprehensive testing

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch: `git checkout -b feature-name`
3. Commit changes: `git commit -am 'Add feature'`
4. Push to branch: `git push origin feature-name`
5. Submit a pull request

## 📝 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🆘 Troubleshooting

### Common Issues

**Build Errors**:
- Ensure all header files are properly included
- Check for missing dependencies
- Verify C++98 compatibility

**Runtime Issues**:
- Verify port is not already in use
- Check file permissions for config files
- Ensure proper network configuration

### Debug Mode

Compile with debug flags for troubleshooting:
```bash
make debug
```

## 📧 Contact

For questions or support, please open an issue in the repository.