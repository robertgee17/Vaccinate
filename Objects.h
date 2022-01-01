#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>

// This file contains the objects the autograder framework uses
// to represent routes and shipments.  If these don't contain all
// the information you need to track, create a new class or a
// subclass to hold your versions.  Don't edit this file!


struct Route {
  unsigned int id;     // The unique ID of this route.
  std::string  city1;  // The name of the city at one end of this route.
  std::string  city2;  // The name of the city at the other end of the route.
  unsigned int days;   // The number of days it takes to travel this route.
  unsigned int load;   // The number of doses that fit in one vehicle.
  unsigned int cost;   // The price you pay to ship one load.
};

struct Shipment {
  unsigned int route_id;  // The ID of the route to use.
  std::string  source;    // The name of the starting city.
  unsigned int day;       // The day this shipment will go out.
  unsigned int doses;     // The number of doses to send.
};

#endif
