#!/bin/bash
# Kill any existing ircserv processes and start fresh

# Kill existing ircserv processes
pkill -f ircserv || true

# Wait a moment for cleanup
sleep 1

# Start the server
echo "Starting IRC server..."
./ircserv 6667 aaaa