#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  void Pid(int n);
  int Pid () const;                             
  std::string User();                     
  std::string Command();                  
  float CpuUtilization() const;               
  std::string Ram();                   
  long int UpTime()const;                      
  
 private:
    int pid;
};

#endif
