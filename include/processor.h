#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

using std::vector;

class Processor {
 public:
  float Utilization();

 private:
  vector<double> GetCpuVector();
  vector<double> prevVec;
  vector<double> currVec;
};
#endif