#include <dirent.h>
#include <unistd.h>
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
    linestream >> os >> kernel >> version;
  }
  return version;
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
float LinuxParser::MemoryUtilization() { 
  string key, unit;
  long value;
  long mem_total, mem_free;
  float utilz;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> key >> value >> unit)
    {
      if (key == "MemTotal:") {mem_total = value;}
      if (key == "MemFree:") {mem_free = value;}
    }
  }
  utilz = (mem_total - mem_free) *1. / mem_total * 1.;
  return utilz;
 }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime_syetem;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (std::getline(stream, line)){
    std::istringstream linestream(line);
    linestream >> uptime_syetem;
  }
  return uptime_syetem;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return ActiveJiffies() + IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  std::ifstream fpid_jif(kProcDirectory + to_string(pid) + kStatFilename);
  long value;

  string line, utime, stime, cutime, cstime;
  if (fpid_jif.is_open()) {
    getline(fpid_jif, line);
    std::istringstream jif_str(line);
    int count = 1;
    string temp;
    while (count <= 13) {
      jif_str >> temp;
      count++;
    }
    jif_str >> utime >> stime >> cutime >> cstime;
  }
  value = std::stol(utime) + std::stol(stime) + std::stol(cutime) +
          std::stol(cstime);
  return value;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> values = CpuUtilization();
  return (std::stol(values[kUser_]) + 
          std::stol(values[kNice_]) +
          std::stol(values[kSystem_]) +
          std::stol(values[kIdle_]) +
          std::stol(values[kSoftIRQ_]) +
          std::stol(values[kSteal_]) +
          std::stol(values[kGuest_]) +
          std::stol(values[kGuestNice_]));
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> values = CpuUtilization();
  return (std::stol(values[kIdle_]) + std::stol(values[kIOwait_]));
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpu_utilz;
  string line, temp;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> temp;
    int i = 0;
    while (i < 10) {
      linestream >> temp;
      cpu_utilz.emplace_back(temp);
      i++;
    }
  }
  return cpu_utilz;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key;
  int processes, value;
  std::ifstream filestream (kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream stream(line);
      while(stream>>key>>value){
        if (key=="processes"){ processes=value;}
      }
    }
  }  
  return processes; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key;
  int runningProcesses, value;
  std::ifstream filestream (kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream stream(line);
      while(stream >> key >> value){
        if (key == "procs_running"){runningProcesses = value;}
      }
    }
  }
  return runningProcesses;  
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    if (line.length() >= 40) return line.substr(0, 40);
    else return line;
    //return line;
  }
  return nullptr;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string key, ram;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> key){
      if (key == "VmData:") // used VmData instead of VmSize, because VmSize is the sum of all the virtual memory
      {
        linestream >> ram;
        return to_string(stof(ram) / 1024);
      }
    }
  }
  return "0";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> key >> value){
      if (key == "Uid") break;
    }
  }
  return value; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::ifstream user_file(kPasswordPath);
  string user_value;
  if (user_file.is_open()) {
    string uid = Uid(pid);
    string line, key, temp;
    while (getline(user_file, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream user_string(line);
      user_string >> user_value >> temp >> key;
      if (key == uid) {
        return user_value;
      }
    }
  }
  return user_value;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string value;
  string line;
  int count = 1;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> value && count <22){
      ++count;
    }
  }  
  return LinuxParser::UpTime() - (stol(value) / sysconf(_SC_CLK_TCK));
}