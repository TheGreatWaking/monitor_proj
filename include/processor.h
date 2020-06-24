#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  long prevIdle = 0;
  long prevNonIdle = 0;
  long Idle;
  long NonIdle;
};

#endif