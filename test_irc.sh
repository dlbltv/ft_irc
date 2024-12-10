#!/bin/bash

# Function to simulate a client interaction
connect_client() {
  client_name=$1
  commands=$2
  {
    echo "pass ps"
    sleep 1
    echo "nick $client_name"
    sleep 1
    echo "user $client_name * $client_name"
    sleep 1
    echo "$commands"
    echo "Waiting for QUIT signal..."
    while true; do
      read -p "Type QUIT to disconnect $client_name: " user_quit
      if [[ "$user_quit" == "QUIT" ]]; then
        echo "quit :Goodbye from $client_name"
		sleep 1
        break
      fi
    done
  } | nc localhost 6987 &
}

echo "Server is ready! Starting tests."

# Simulate multiple clients with predefined commands
connect_client "A" "join #1"
connect_client "B" "join #1"
connect_client "C" "join #1"
connect_client "D" "join #1"
connect_client "E" "join #1"
connect_client "F" "join #1"
connect_client "G" "join #1"
connect_client "H" "join #1"
connect_client "I" "join #1"

# Wait for all clients to finish
wait

echo "All clients have finished their interactions."
