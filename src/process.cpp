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
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    long ProcJiffies = LinuxParser::ActiveJiffies(pid);
    long TotJiffies = LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
    cpu_utiliz = (float)(ProcJiffies - prevProcJiffies) / (TotJiffies - prevTotJiffies);
    prevTotJiffies = TotJiffies;
    prevProcJiffies = ProcJiffies;
    return cpu_utiliz; 
}

// TODO: Return the command that generated this process
string Process::Command() { 
    command = LinuxParser::Command(pid);
    return command;
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    ram = LinuxParser::Ram(pid); //MB
    return ram;
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    uname = LinuxParser::User(pid);
    return uname;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    uptime = LinuxParser::UpTime(pid);
    return uptime;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return cpu_utiliz > a.cpu_utiliz;
}