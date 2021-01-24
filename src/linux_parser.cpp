#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <numeric>
#include <stdlib.h>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Return OS version string
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

// Return kernel string
string LinuxParser::Kernel() {
  string kernel,os,ver;
  string line;
  std::ifstream ip_file(kProcDirectory + kVersionFilename);
  if (ip_file.is_open()) {
    std::getline(ip_file, line);
    std::istringstream parse_obj(line);
    parse_obj >> os >> ver >> kernel;
  }
  return kernel;//return kernel string
}

//Return the Pids list
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  int tot_mem,free_mem = 0;//in KBs
  float Mem_util = 0;//fraction of memory utilization
  string line,key;
  std::ifstream ip_file(kProcDirectory + kMeminfoFilename);
  if(ip_file.is_open())
  {
    while (getline(ip_file,line))
    {
      std::replace(line.begin(),line.end(),':',' ');
      std::istringstream parse_obj(line);
      if(parse_obj >> key && (key == "MemTotal" || key == "MemFree"))
      {
          if(key == "MemTotal")
            parse_obj >> tot_mem; 
          else
            parse_obj >> free_mem;
      }
    }
  }
  Mem_util = free_mem / (tot_mem * 1.0);
  Mem_util = 1 - Mem_util;//used memory
  return Mem_util;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  std::string line;
  long up_time = 0;
  std::ifstream ip_file(kProcDirectory + kUptimeFilename);
  if(ip_file.is_open())
  {
    std::getline(ip_file,line);
    std::istringstream parse_str(line);
    parse_str >> up_time;
  }
  return up_time;
}

// Read and return the number of CPU utilization
float LinuxParser::cpu_util(int pid) { 
  std::string line;
  char a[100];
  vector <int> ticks;//ticks
  ticks.clear();
  std::ifstream ip_file(kProcDirectory + to_string(pid) + kStatFilename);
  if(ip_file.is_open())
  {
    std::getline(ip_file,line);
    std::istringstream parse_obj(line);
    int i;
    for(i=0;i<13;i++)
    {
       parse_obj >> a;//utime 
    }
    for(i=0;i<4;i++)
    {
        parse_obj >> a;
        ticks.push_back(atoi(a));//utime,14 + stime,15 + cutime,16 + cstime,17
    }
    for(i=0;i<5;i++)
    {
      parse_obj >> a;
    }
    int hertz = sysconf(_SC_CLK_TCK);
    long int time_proc = LinuxParser::UpTime() - atoi(a)/hertz;//accurate up time in sec
    int process_t = (std::accumulate(ticks.begin(),ticks.end(),0))/ hertz;//process time in seconds
    float util = process_t / (time_proc * 1.0);
    return util;
  }
  return 0; 
}

//Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream ip_file(kProcDirectory  + kStatFilename);
  std::string line,key;
  int proc_no = 0;
  if(ip_file.is_open())
  {
    while(std::getline(ip_file,line))
    {
        std::istringstream parse_obj(line);
        if(parse_obj >> key && key == "processes")
        {
          parse_obj >> proc_no;
        }
    }
  }
  return proc_no;//total number of processes
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::ifstream ip_file(kProcDirectory  + kStatFilename);
  std::string line,key;
  int run_proc = 0;//count of running processes
  if(ip_file.is_open())
  {
    while(std::getline(ip_file,line))
    {
        std::istringstream parse_str(line);
        if(parse_str >> key && key == "procs_running")
        {
          parse_str >> run_proc;
        }
    }
  }
  return run_proc;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  std::ifstream ip_file(kProcDirectory + to_string(pid) + kCmdlineFilename); 
  std::string line;
  if(ip_file.is_open())
    getline(ip_file,line);
  return line;
  }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
    std::string line,key;
    long int val = 0;
    std::ifstream ip_file(kProcDirectory + to_string(pid) + kStatusFilename);
    if(ip_file.is_open())
    {
      while(getline(ip_file,line)){
      std::replace(line.begin(),line.end(),':',' ');
      std::istringstream parse_obj(line);
      if(parse_obj >> key && key == "VmSize")
      {
          parse_obj >> val;
          val = val / 1000;//convert to MB
      }
    }
    }
    return (to_string(val));
  }

//  Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
   string uid;
   string str,line;
   std::ifstream ip_file(kProcDirectory + std::to_string(pid) + kStatusFilename);
   if(ip_file.is_open())
    {
     while(std::getline(ip_file,line))
      {
          std::replace(line.begin(),line.end(),':',' ');
          std::istringstream parse_obj(line);
          if(parse_obj >> str && str == "Uid")
            parse_obj >> uid;
      }
    }
    return uid;
   }

// Read and return the user associated with a process
string LinuxParser::User(string uid) {
      std::string line,uid_search,user,name; 
      std::ifstream ip_file(kPasswordPath);
      if(ip_file.is_open())
      {
        while(std::getline(ip_file,line))
        {
          std::replace(line.begin(),line.end(),':',' ');
          std::istringstream parse(line);
          if(parse >> user >> name >> uid_search && uid_search == uid)
          {
            return user;
          }
        }
      }
      user = {};
      return user;
  }

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::string line;
  char getval[100];
  std::ifstream ip_file(kProcDirectory + to_string(pid) + kStatFilename);//utime,14 + stime,15 + cutime,16 + cstime,17
  if(ip_file.is_open())
  {
    std::getline(ip_file,line);
    std::istringstream parse_obj(line);
    int i;
    for(i=0;i<22;i++)
    {
       parse_obj >> getval;//utime 
    }
    int hertz = sysconf(_SC_CLK_TCK);
    long int strt_time = atoi(getval) / hertz;
    return(LinuxParser::UpTime() - strt_time);
  }
  return 0;
}
