#include "processor.h"
#include <string>
#include <fstream>
#include "linux_parser.h"

using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  float totalTime = LinuxParser::Jiffies();
  float activeTime = LinuxParser::ActiveJiffies();
  return 1.0 * (activeTime / totalTime);
  /*Idle = LinuxParser::IdleJiffies();
  NonIdle = LinuxParser::ActiveJiffies();
  //calculation
  long prevTotal = prevIdle + prevNonIdle;
  long Total = Idle + NonIdle;
  //differentiate
  long TotalDelta = Total - prevTotal;
  long IdleDelta = Idle - prevIdle;
  
  prevIdle = Idle;
  prevNonIdle = NonIdle;
  return (float)(TotalDelta - IdleDelta) / TotalDelta; */
}