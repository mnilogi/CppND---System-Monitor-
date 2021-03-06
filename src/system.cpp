#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

#include "linux_parser.h"

Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
    vector <int> all_proc{};//vector of pids
    all_proc = LinuxParser::Pids();  
    processes_.clear();
    for(int i : all_proc)
    {
        Process a;
        a.Pid(i);
        processes_.push_back(a);
    }
    std::sort(processes_.begin(),processes_.end(),[](Process &a,Process &b){return(a.CpuUtilization() > b.CpuUtilization());});
    return processes_; 
}

std::string System::Kernel() { 
    return(LinuxParser::Kernel());
 }

float System::MemoryUtilization() { 
    return(LinuxParser::MemoryUtilization());
}

std::string System::OperatingSystem() { 
        return(LinuxParser::OperatingSystem());
    }

int System::RunningProcesses() { 
        return(LinuxParser::RunningProcesses());
 }

int System::TotalProcesses() { 
        return(LinuxParser::TotalProcesses());
}

long int System::UpTime() { 
    return LinuxParser::UpTime();
}
