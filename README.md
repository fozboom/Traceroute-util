# Traceroute Utility

This project is a command-line utility to trace the route packets take from your system to a target host. It utilizes raw ICMP (Internet Control Message Protocol) sockets to send packets and display information about the path they traverse.

## Features
- Customizable maximum hops (`-m`)
- Option to save results to a file (`-f`)
- Resolves and traces IP addresses to any given hostname

## Requirements
- C++17 or later
- `sudo` access (due to raw socket operations)
- CMake (for building)

## Build Instructions
1. Clone the repository:
   ```bash
   git clone <repository-url>

2. Create a build directory:
   ```bash
   mkdir build
   cd build

3. Run CMake:
   ```bash
   cmake ..

4. Build the project:
   ```bash
    make
   
## Usage 

1. Basic usage:
   ```bash
   sudo ./traceroute <hostname>
   Example:
   sudo ./traceroute google.com

2. Custom maximum hops:
   ```bash
    sudo ./traceroute <hostname> -m <max-hops> <hostname>
    Example:
    sudo ./traceroute google.com -m 20

3. Save results to a file:
    ```bash
     sudo ./traceroute <hostname> -f <filename>
     Example:
     sudo ./traceroute google.com -f results.txt