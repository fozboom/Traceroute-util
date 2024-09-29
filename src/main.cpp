#include <Traceroute.hpp>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <HostnameResolutionException.hpp>
#include <SocketCreationException.hpp>
#include <algorithm>  // For std::all_of
#include <cctype>     // For std::isdigit

void PrintHelp() {
  std::cout << "Usage: traceroute <hostname> [-m max_hops] [-o output_file]\n";
  std::cout << "Options:\n";
  std::cout << "  --help       Show this help message\n";
  std::cout <<
      "  -m           Specify the maximum number of hops (default: 30)\n";
  std::cout << "  -f           Output results to the specified file\n";
}

bool IsValidNumber(const std::string& str) {
  return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

int ParseMaxHops(const char* arg) {
  if (!IsValidNumber(arg)) {
    std::cerr << "Error: max_hops should be a positive integer.\n";
    std::exit(1);
  }

  int max_hops = std::stoi(arg);
  if (max_hops <= 0 || max_hops > 255) {
    std::cerr << "Error: max_hops must be between 1 and 255.\n";
    std::exit(1);
  }

  return max_hops;
}


void RunTraceroute(const std::string& hostname, int max_hops,
                   const std::string& output_filename,
                   bool output_to_file) {
  try {
    Traceroute traceroute(hostname, max_hops);

    traceroute.RunTraceroute();
    if (output_to_file) {
      traceroute.SaveResultsToFile(output_filename);
    }
  } catch (const HostnameResolutionException& e) {
    std::cerr << "Hostname resolution error: " << e.what() << std::endl;
    std::exit(1);
  } catch (const SocketCreationException& e) {
    std::cerr << "Socket creation error: " << e.what() << std::endl;
    std::exit(1);
  } catch (const std::exception& e) {
    std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
    std::exit(1);
  }
}

void ParseArguments(int argc, char* argv[], std::string& hostname,
                    int& max_hops, std::string& output_filename,
                    bool& output_to_file) {
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help") {
      PrintHelp();
      std::exit(0);
    } else if (arg == "-m" && i + 1 < argc) {
      max_hops = ParseMaxHops(argv[++i]);
    } else if (arg == "-f" && i + 1 < argc) {
      output_filename = argv[++i];
      output_to_file = true;
    } else if (arg == "-f" && i + 1 >= argc) {
      std::cerr << "Error: Output file name required after -f flag.\n";
      std::exit(1);
    } else if (hostname.empty()) {
      hostname = arg;
    } else {
      std::cerr << "Error: Unrecognized argument: " << arg << "\n";
      std::exit(1);
    }
  }
}


int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Error: Hostname required. Use --help for usage info.\n";
    return 1;
  }

  std::string hostname;
  int max_hops = 30;
  std::string output_filename;
  bool output_to_file = false;

  ParseArguments(argc, argv, hostname, max_hops, output_filename,
                 output_to_file);

  if (hostname.empty()) {
    std::cerr << "Error: Hostname required. Use --help for usage info.\n";
    return 1;
  }

  RunTraceroute(hostname, max_hops, output_filename, output_to_file);
  return 0;
}