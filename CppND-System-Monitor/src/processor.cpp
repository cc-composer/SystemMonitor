#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <dirent.h>

using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{ 
  //User -- Nice -- System -- Idle -- Iowait -- irq -- softirq -- steal -- guest -- guest_nice
  //Idle = idle + iowait
  //NonIdle = user + nice + system + irq + softirq + steal
  //Total = Idle + NonIdle
  //CPU_Percentage = (Total - Idle) / Total
  string type;
  float user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  while (stream >> type >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice) 
  {
  	if (type == "cpu") 
    {
		float idleT = idle + iowait;
      	float nIdleT = user + nice + system + irq + softirq + steal;
      	float total = idleT + nIdleT;
      	return (total - idleT) / total;
  	}
  }
  
  return 0.0; 
}