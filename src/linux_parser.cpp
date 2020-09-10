#include "linux_parser.h"
#include "format.h"

#include <dirent.h>
#include <unistd.h>

#include <cmath>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::string line, key, value;
  double memTotal, memFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        memTotal = std::stod(value);
      }
      if (key == "MemFree:") {
        memFree = std::stod(value);
        break;
      }
    }
    return (memTotal - memFree) / memTotal;
  }
  return -1;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  std::string line, upTimeStr, tmp;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> upTimeStr >> tmp;
    return std::stod(upTimeStr);
  }
  return -1;
}

// TODO: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization(int pid) {
  vector<long> cpuValues;
  std::string line, tmp;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    int c = 1;
    std::getline(filestream, line);
    std::istringstream line_stream(line);
    while (getline(line_stream, tmp, ' ') && c < 23) {
      if (c == 14 || c == 15 || c == 16 || c == 17 || c == 22) {
        cpuValues.push_back(std::stod(tmp));
      }
      c++;
    }
  }
  return cpuValues;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::string line, key, value;
  int totalProcesses;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        totalProcesses = std::stoi(value);
        break;
      }
    }
    return totalProcesses;
  }
  return -1;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::string line, key, value;
  int runningProcesses;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        runningProcesses = std::stoi(value);
        break;
      }
    }
    return runningProcesses;
  }
  return -1;
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::string line, comamnd;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::string line, key, value;
  value = "0";
  std::ifstream filestream1(kProcDirectory + std::to_string(pid) +
                            kStatusFilename);
  if (filestream1.is_open()) {
    while (std::getline(filestream1, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        break;
      }
    }
  }
  return Format::ConvertRam(value);
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::string line, key, value, Uid;
  std::ifstream filestream1(kProcDirectory + std::to_string(pid) +
                            kStatusFilename);
  if (filestream1.is_open()) {
    while (std::getline(filestream1, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        Uid = value;
        break;
      }
    }
  }
  return Uid;
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  std::string Uid = LinuxParser::Uid(pid);
  std::string line, tmpName, tmp, tmpId, Uname;
  std::ifstream filestream2(kPasswordPath);
  if (filestream2.is_open()) {
    while (std::getline(filestream2, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');  // replace ':' by ' '
      std::istringstream linestream(line);
      linestream >> tmpName >> tmp >> tmpId;
      if (tmpId == Uid) {
        Uname = tmpName;
        break;
      }
    }
  }
  return Uname;
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::string line, tmp, upTimeStr;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    int counter = 1;
    std::getline(filestream, line);
    std::istringstream line_stream(line);
    while (getline(line_stream, tmp, ' ')) {
      if (counter == 22) {
        long clock_ticks = std::stol(tmp);
        long seconds = long(clock_ticks / sysconf(_SC_CLK_TCK));
        return seconds;
      }
      counter++;
    }
  }
  return -1;
}