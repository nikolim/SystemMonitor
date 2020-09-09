#include "process.h"
#include "linux_parser.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
  pid_ = pid;
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpuUtilization_; }

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return upTime_; }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  // Using RAM for comparision between different processes
  // return std::stoi(a.ram_) < std::stoi(ram_);
  return a.cpuUtilization_ < cpuUtilization_;
}

void Process::Update() {
  user_ = LinuxParser::User(pid_);
  cpuUtilization_ = LinuxParser::CpuUtilization(pid_);
  upTime_ = LinuxParser::UpTime(pid_);
  command_ = LinuxParser::Command(pid_);
  ram_ = LinuxParser::Ram(pid_);
}