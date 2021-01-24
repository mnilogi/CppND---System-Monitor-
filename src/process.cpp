#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

void Process::Pid(int n){
    pid = n;
}
int Process::Pid() const{ 
    return pid;
}
float Process::CpuUtilization() const{
    return(LinuxParser::cpu_util(Pid()));    
}
string Process::Command() { 
    return(LinuxParser::Command(Pid()));
 }
string Process::Ram() { 
    return(LinuxParser::Ram(Pid()));
}
string Process::User() { 
    return(LinuxParser::User(LinuxParser::Uid(Pid())));
}
long int Process::UpTime() const{ 
    return(LinuxParser::UpTime(Pid()));
}
