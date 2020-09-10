#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) { pid_ = pid; }

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 

  long uptime = LinuxParser::UpTime();
  long hertz = sysconf(_SC_CLK_TCK);

  vector<long> cpuVec = LinuxParser::CpuUtilization(pid_);
  long utime = cpuVec[0];
  long stime = cpuVec[1];
  long cutime = cpuVec[2];
  long cstime = cpuVec[3];
  long starttime = cpuVec[4];

  float total_time = (utime + stime + cutime + cstime) / hertz;
  float seconds = uptime - (starttime / hertz);
  float cpu_usage = (total_time / hertz) / seconds ;

  return cpu_usage; 
  }

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
  //return std::stoi(a.ram_) < std::stoi(ram_);

  // Using CPU for comparision between different processes
  return a.cpuUtilization_ < cpuUtilization_;
}

void Process::Update() {

  // Can be read directly from the system
  user_ = LinuxParser::User(pid_);
  upTime_ = LinuxParser::UpTime(pid_);
  command_ = LinuxParser::Command(pid_);
  ram_ = LinuxParser::Ram(pid_);

  // Must be calculated
  cpuUtilization_ = Process::CpuUtilization();
}