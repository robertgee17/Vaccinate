#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "Objects.h"

#include <cstdint>
#include <map>
#include <vector>

// Thrown when the validator detects an error.
struct validation_error: public std::runtime_error {
  validation_error(const std::string& message): std::runtime_error(message) {}
};


class Validator {
  struct City {
    std::string   name;
    bool          factory;
    unsigned int  population;
    unsigned int  vaccines;
    unsigned int* arrivals;
  };

private:
  std::map<std::string, City> mCities;
  std::vector<Route> mRoutes;

  unsigned int mDeadline;
  uint64_t     mEstimate;
  uint64_t     mExpenses;
  bool         mVerbose;

private:
  City&  city(const std::string& name);
  City&  destination(const Shipment& shipment);
  void   log(const std::string& message) const;
  Route& route(unsigned int id);

public:
  Validator(
    unsigned int                        deadline,
    unsigned int                        estimate,
    std::map<std::string, unsigned int> cities,
    std::vector<std::string>            factories,
    std::vector<Route>                  routes,
    bool                                verbose
  );
  ~Validator();

  unsigned int estimate() const;
  unsigned int expenses() const;

  void summarize() const;
  void validate(std::vector<Shipment>& shipments);
};

#endif
