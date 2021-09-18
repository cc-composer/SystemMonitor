#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  string type, key, size;
  float memTotal, memFree;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  while (stream >> type >> key >> size) 
  {
  	if (type == "MemTotal:") 
    {
    	memTotal = std::stof(key);
  	}
    
    if (type == "MemFree:") 
    {
    	memFree = std::stof(key);
  	}
  }
  return (memTotal - memFree) / memTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{   
  long up, idle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if(stream.is_open())
  {
      std::getline(stream, line);
      std::istringstream sstream(line);        
  	  sstream >> up >> idle;
      return up;
  }

  return 0;
}

// TODO: Read and return the number of jiffies for the system
//long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
//long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
//long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
//long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
//vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  string type;
  int proc;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  
  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::istringstream sstream(line);        
  	  while (sstream >> type >> proc) 
  	  {
  		if (type == "processes") 
        {
          return proc;
  		}
  	  }
    }
  }

  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  string type; 
  int procR;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  
  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::istringstream sstream(line);        
  	  while (sstream >> type >> procR) 
  	  {
  		if (type == "procs_running") 
        {
          return procR;
  		}
  	  }
    }
  }
  
  return 0;
}

float LinuxParser::CPU(int pid)
{
  float utime, stime, cutime, cstime, starttime = 0;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  
  if(stream.is_open())
  {
  	std::getline(stream, line);
    std::istringstream sstream(line);
    string placeholder;
    long upTime = LinuxParser::UpTime(pid);
    
    for(auto i = 0; i < 13; i++) { sstream >> placeholder; }
    sstream >> utime >> stime >> cutime >> cstime;
    for(auto i = 0; i < 4; i++) { sstream >> placeholder; }
    sstream >> starttime;
    
    float total_time = utime + stime + cutime + cstime;
    float seconds = upTime - (starttime / sysconf(_SC_CLK_TCK));
    
    return (total_time / sysconf(_SC_CLK_TCK)) / seconds;
  }
  
  return 0.0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{ 
  string line;
  string command;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) 
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> command;
    return command;
  }
  
  return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{ 
  string type; 
  int memory;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  
  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::istringstream sstream(line);        
  	  while (sstream >> type >> memory) 
  	  {
  		if (type == "VmSize:") 
        {
          return to_string(memory/1024);
  		}
  	  }
    }
  }
  
  return " ";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{ 
  string type;
  int UID{0}, a{0}, b{0}, c{0};
  string line;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatusFilename);
  
  if (stream.is_open()) 
  {
    while (std::getline(stream, line)) 
    {
      std::istringstream linestream(line);
      while (linestream >> type >> UID >> a >> b >> c) 
      {
        if (type == "Uid:") 
        {
          return LinuxParser::User(UID);
        }
      }
    }
  }
  
  return LinuxParser::User(UID);
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int UID) 
{ 
  std::string line, name, x;
  int id;

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> x >> id) 
      {
        if (id == UID) 
        {
          return name;
        }
      }
    }
  }
  
  return to_string(UID);
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{ 
  long upTime;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  
  if(stream.is_open())
  {
  	std::getline(stream, line);
    std::istringstream sstream(line);
    string placeholder;
    
    for(auto i = 0; i < 21; i++) { sstream >> placeholder; }
    sstream >> upTime;
    
    return LinuxParser::UpTime() - (upTime / sysconf(_SC_CLK_TCK));
  }  
  
  return 0;
}
