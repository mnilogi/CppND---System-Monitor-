#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
class Processor {
 public:
  float Utilization();  
  void TotalJiffies();
  void IdleJiffies();
  void ReadAllJiffies();

 private:
  std::vector<long int> jiffies_val{};
  long int totaljiffies;
  long int idlejiffies;
};

#endif
