#include "Validator.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

// Helper function for sorting shipments.
static bool daycmp(const Shipment& a, const Shipment& b) {
  return a.day < b.day;
}

// Helper function for shorter log lines.
static std::string s(unsigned int value) {
  return std::to_string(value);
}


Validator::Validator(
    unsigned int                        deadline,
    unsigned int                        estimate,
    std::map<std::string, unsigned int> cities,
    std::vector<std::string>            factories,
    std::vector<Route>                  routes,
    bool                                verbose
) {
  mDeadline = deadline;
  mEstimate = estimate;
  mVerbose  = verbose;
  mRoutes   = routes;
  mExpenses = 0;

  for(const auto& pair: cities) {
    City city;
    city.name       = pair.first;
    city.factory    = false;
    city.population = pair.second;
    city.arrivals   = new unsigned int[deadline+1]();
    city.vaccines   = 0;

    mCities[pair.first] = city;
  }

  for(const auto& name: factories) {
    city(name).factory = true;
  }
}

Validator::~Validator() {
  for(auto& pair: mCities) {
    delete [] pair.second.arrivals;
  }
}

Validator::City& Validator::city(const std::string& name) {
  auto itr = mCities.find(name);
  if(itr == mCities.end()) {
    throw validation_error("No such city: " + name);
  }

  return itr->second;
}

Validator::City& Validator::destination(const Shipment& shipment) {
  Route& rte = route(shipment.route_id);
  if(shipment.source == rte.city1) {
    return city(rte.city2);
  }
  if(shipment.source == rte.city2) {
    return city(rte.city1);
  }

  throw validation_error("Route " + s(shipment.route_id) + " does not go to " + shipment.source + ".");
}

unsigned int Validator::estimate() const {
  return mEstimate;
}

unsigned int Validator::expenses() const {
  return mExpenses;
}

void Validator::log(const std::string& message) const {
  if(mVerbose) {
    std::cout << message << "\n";
  }
}

Route& Validator::route(unsigned int id) {
  if(id >= mRoutes.size()) {
    throw validation_error("Invalid route ID: " + s(id));
  }

  return mRoutes[id];
}

void Validator::summarize() const {
  log("");
  std::cout << "All cities received enough vaccine.\n";
  std::cout << "Estimated shipping costs: " << std::setw(10) << mEstimate << "\n";
  std::cout << "Actual shipping costs:    " << std::setw(10) << mExpenses << "\n";
}

void Validator::validate(std::vector<Shipment>& shipments) {
  // Make sure all the shipments are ordered by day:
  std::sort(shipments.begin(), shipments.end(), daycmp);
  log("Day 0 (morning): Go!");

  unsigned int i   = 0;
  unsigned int day = 0;
  while(day < mDeadline) {
    log("Day " + s(day) + " (afternoon):");
    for(; i < shipments.size(); ++i) {
      Shipment& shipment = shipments[i];
      if(shipment.day != day) break;

      Route& rte = route(shipment.route_id);
      City&  src = city(shipment.source);
      City&  dst = destination(shipment);

      if(src.vaccines < shipment.doses && !src.factory) {
        throw validation_error("Not enough doses in " + src.name + " to ship.");
      }

      if(!src.factory) {
        src.vaccines -= shipment.doses;
      }

      unsigned int arr = day + rte.days;
      if(arr > mDeadline) {
        log(" ! Shipment from " + src.name + " will arrive in " + dst.name + " after the deadline.");
      }
      else {
        log(" - " + s(shipment.doses) + " doses leave " + src.name + " for " + dst.name + ".");
        dst.arrivals[arr] += shipment.doses;
      }

      unsigned int cost = (shipment.doses + rte.load - 1) / rte.load * rte.cost;
      mExpenses += cost;
    }

    day += 1;
    log("Day " + s(day) + " (morning):");
    for(auto& pair: mCities) {
      City& city = pair.second;
      unsigned int doses = city.arrivals[day];

      if(doses > 0) {
        log(" - " + s(doses) + " doses arrive in " + city.name + ".");
        city.vaccines += doses;
      }
    }
  }

  log("Day " + s(day) + " (afternoon): Deadline!");
  // Make sure all cities got enough doses:
  for(auto& pair: mCities) {
    City& city = pair.second;
    if(city.vaccines < city.population && !city.factory) {
      throw validation_error("Not enough doses in " + pair.first + ".");
    }
  }
}
