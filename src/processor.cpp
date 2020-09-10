#include "processor.h"

#include <chrono>
#include <thread>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::vector;

vector<double> Processor::GetCpuVector() {
  vector<double> cpuValues;
  string fstLine, tmp;
  string filepath = (LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::ifstream filestream(filepath);

  if (filestream.is_open()) {
    std::getline(filestream, fstLine);
    std::istringstream linestream(fstLine);
    while (linestream >> tmp) {
      try {
        cpuValues.push_back(std::stod(tmp));
      } catch (std::invalid_argument& e) {
      }
    }
  }
  return cpuValues;
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  // 0.user, 1.nice, 2.system, 3.idle, 4.iowait, 5.irq, 6.softirq, 7.steal, 8.guest,
  // 9.guest_nice
  Processor::currVec = Processor::GetCpuVector();

  if (!Processor::prevVec.empty()) {
    double prevIdle = Processor::prevVec[3] + Processor::prevVec[4];
    double idle = Processor::currVec[3] + Processor::currVec[4];

    double prevNonIdle = Processor::prevVec[0] + Processor::prevVec[1] +
                         Processor::prevVec[2] + Processor::prevVec[5] +
                         Processor::prevVec[6] + Processor::prevVec[7];
    double nonIdle = Processor::currVec[0] + Processor::currVec[1] +
                     Processor::currVec[2] + Processor::currVec[5] +
                     Processor::currVec[6] + Processor::currVec[7];

    double prevTotal = prevIdle + prevNonIdle;
    double total = idle + nonIdle;

    double totald = total - prevTotal;
    double idled = idle - prevIdle;

    float cpu_percentage = (totald - idled) / totald;

    Processor::prevVec = Processor::currVec;
    return cpu_percentage;
  }
  Processor::prevVec = Processor::currVec;
  return 0.0;
}