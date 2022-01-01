#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Objects.h"

#include <map>
#include <vector>

// This file describes the Scheduler interface you'll need to implement.
// Don't edit this file!  Your code goes in the MyScheduler.* files.

class Scheduler {
public:
  static Scheduler* create(
    unsigned int                        deadline,   // The number of days until the deadline.
    std::map<std::string, unsigned int> cities,     // The names and populations of all cities.
    std::vector<std::string>            factories,  // The names of the cities that contain factories.
    std::vector<Route>                  routes      // All available shipping routes.
  );

public:
  virtual std::vector<Shipment> schedule() = 0;
  virtual ~Scheduler() {}
};

#endif
