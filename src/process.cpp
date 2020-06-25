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

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    float total_time = LinuxParser::ActiveJiffies(pid_);
    float seconds = LinuxParser::UpTime() - (LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK));
    cpu_utiliz_ = (total_time * 1.0 / sysconf(_SC_CLK_TCK) / seconds);
    //long TotJiffies = LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
    //cpu_utiliz = (float)(ProcJiffies - prevProcJiffies) / (TotJiffies - prevTotJiffies);
    //prevTotJiffies = TotJiffies;
    //prevProcJiffies = ProcJiffies;
    return cpu_utiliz_; 
}

// TODO: Return the command that generated this process
string Process::Command() { 
    command_ = LinuxParser::Command(pid_);
    return command_;
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    ram_ = LinuxParser::Ram(pid_); //MB
    return ram_;
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    user_ = LinuxParser::User(pid_);
    return user_;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    uptime_ = LinuxParser::UpTime(pid_);
    return uptime_;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return cpu_utiliz_ > a.cpu_utiliz_;
}