#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  long prev_total_ = 0;
  long prev_total_idle = 0;
  //long Idle;
  //long NonIdle;
};

#endif