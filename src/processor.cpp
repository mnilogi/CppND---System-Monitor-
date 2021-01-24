#include "processor.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <numeric>
#include "linux_parser.h"
// Return the aggregate CPU utilization
float Processor::Utilization(){
    float proc_util;
    jiffies_val.clear();
    ReadAllJiffies();
    IdleJiffies();
    TotalJiffies();
    proc_util = (totaljiffies - idlejiffies)/(totaljiffies * 1.0);
    return proc_util; 
}
//Read the new set of values from the Processor utilization file
void Processor::ReadAllJiffies()
{
    std::string line,key; 
    long int jiffies_count;
    std::ifstream ip_file(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if(ip_file.is_open())
    {
        while (std::getline(ip_file,line))
        {
            std::istringstream parse_obj(line);
            if(parse_obj >> key && key == "cpu")
            {
                while(parse_obj >> jiffies_count){
                    jiffies_val.push_back(jiffies_count);
                }
            }
        }
    }
}
//Return number of idlejiffies
void Processor::IdleJiffies()
{
      idlejiffies = jiffies_val[LinuxParser::kIdle_] + jiffies_val[LinuxParser::kIOwait_];
}
//Return number of totaljiffies
void Processor::TotalJiffies()
{
    totaljiffies = std::accumulate(jiffies_val.begin(),jiffies_val.end()-2,0);//last 2 guest values are included in user value
}
