#include <iostream> 
#include <string>  
#include <winsock2.h>  
#include <ws2tcpip.h>  
#include <fstream>  
#include <vector> 
#include <map>
#include <ctime>
#include <sstream>
#include <windows.h> 
#include <sys/stat.h>
#include <filesystem>
#include <sys/stat.h>

// HTTP setup
 // Check if there is an incoming HTTP request

// Convert received buffer into a string message
string input(buf);

// Update client activity timestamp (last time seen online)
it->second.last_seen = time(0);

// Increase request counter for this client
it->second.requests++;

// Save message into log (limit message to 200 chars for safety/log size control)
messages_log.push_back({
    it->second.ip,
    input.substr(0, 200),
    get_time_now()
});

// Prepare response string (to be filled later based on command)
string response = "";

// Check if current client is admin
bool is_admin = (id == admin_client);

// If message starts with '/', treat it as a command
if (input[0] == '/') {
    stringstream ss(input);
    string cmd, arg1, arg2;

    // Extract command and first argument
    ss >> cmd >> arg1;

    // Read the rest of the line as second argument (if any)
    getline(ss, arg2);

    // Remove leading space from arg2 if it exists
    if (!arg2.empty())
        arg2 = arg2.substr(1);