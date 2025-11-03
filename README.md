# IRC Server

A custom implementation of an Internet Relay Chat (IRC) server written in C++.

## 📋 Table of Contents


## 🎯 Overview

This IRC server is designed to handle multiple client connections simultaneously, supporting standard IRC protocol operations including user registration, channel management, and message routing. The server implements a modular response system with configurable numeric reply templates.

## ✨ Features


## Changelog (2025-11-02)


These changes were applied to improve runtime stability and reduce noisy logging during normal operation. If you are developing on the project and prefer to keep comments or debug prints, restore them from your VCS history or re-run the tool with modifications.

## 📁 Project Structure

```
IRC_server/
├── src/
│   ├── server/server.cpp              # Main server implementation
│   ├── Client/Client.cpp              # Client connection handling  
│   ├── reactor/reactor.cpp            # Event multiplexer
│   ├── Parser/Parser.cpp              # IRC message parser
│   ├── Channel/Channel.cpp            # Channel management
│   ├── JoinCommand/JoinCommand.cpp    # JOIN command handler
│   ├── PrivmsgCommand/PrivmsgCommand.cpp # PRIVMSG command handler
│   ├── UserCommand/UserCommand.cpp    # USER command handler
│   ├── PassCommand/PassCommand.cpp    # PASS command handler
│   ├── nickCommand/nickCommand.cpp    # NICK command handler
│   ├── ListCommand/ListCommand.cpp    # LIST command handler
│   ├── PartCommand/PartCommand.cpp    # PART command handler
│   ├── QuitCommand/QuitCommand.cpp    # QUIT command handler
│   ├── PingCommand/PingCommand.cpp    # PING command handler
│   ├── KickCommand/KickCommand.cpp    # KICK command handler
│   ├── inviteCommand/inviteCommand.cpp # INVITE command handler
│   └── test.cpp                       # Main entry point
├── include/                           # Header files
│   ├── Server.hpp                     # Server class definition
│   ├── Client.hpp                     # Client class definition  
│   ├── Reactor.hpp                    # Reactor pattern implementation
│   ├── Parser.hpp                     # Message parser
│   ├── Channel.hpp                    # Channel management
│   └── [Command headers]              # Individual command headers
├── config/
│   └── numericReplies.txt            # IRC numeric reply templates
├── Makefile                          # Build configuration
└── README.md                         # Project documentation
```

## 🔧 Prerequisites


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
./ircserv <port> <password>
```

**Parameters**:

**Example**:
```bash
./ircserv 6667 test
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

## 📡 Supported IRC Commands

### User Commands

### Channel Commands

### Server Commands

### Standard IRC Numeric Replies

## 🏗️ Architecture

### Core Components

1. **Server**: Main singleton managing client connections, channels, and IRC protocol
2. **Reactor**: Event-driven I/O multiplexer using epoll for high performance
3. **Client**: Individual connection handler with message buffering and parsing
4. **Parser**: IRC protocol message parser and validator
5. **Channel**: Channel management with member tracking and message broadcasting
6. **Command System**: Modular command handlers using factory pattern

### Design Patterns Used


### Key Classes

```cpp
class Server {
    static Server& getInstance();
    void handle_event(epoll_event ev);
    void callCommand(std::string& cmd, std::map<std::string, std::string>& params, Client& sender);
};

class Reactor {
    static Reactor& getInstance();
    void registre(epoll_event ev, IEventHandler* handler);
    void Run();
};

class Client : public IEventHandler {
    void handle_event(epoll_event e);
    void addMsg(std::string msg);
};
```

### Performance Features

## 🔧 Development

### Adding New Commands

1. Create command handler in appropriate source file
2. Add command parsing logic
3. Update response templates in `config/numericReplies.txt`
4. Test with IRC client

### Code Style Guidelines


## 🧪 Testing

### Quick Start Testing
```bash
# Start server
./ircserv 6667 test

# Interactive testing with netcat
nc localhost 6667
# Then type:
PASS test
USER alice 0 * :Alice Smith
NICK alice
JOIN #general
PRIVMSG #general :Hello World!
LIST
PART #general :Goodbye!
QUIT
```

### Multi-Client Testing
```bash
# Terminal 1 - Alice
nc localhost 6667 << 'EOF' &
PASS test
USER alice 0 * :Alice
NICK alice
JOIN #chat
PRIVMSG #chat :Alice here!
EOF

# Terminal 2 - Bob
nc localhost 6667 << 'EOF'
PASS test
USER bob 0 * :Bob
NICK bob
JOIN #chat
PRIVMSG #chat :Bob joined!
EOF
```

### Using Standard IRC Clients
```bash
# HexChat, irssi, weechat
# Server: localhost
# Port: 6667
# Password: test
```

### Expected Responses

## � Current Status: PRODUCTION READY

**Fully Implemented & Tested**:

**Evaluation Ready**:

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

**Runtime Issues**:

### Debug Mode

Compile with debug flags for troubleshooting:
```bash
make debug
```

## 📧 Contact

For questions or support, please open an issue in the repository.