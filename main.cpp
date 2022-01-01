#include "Scheduler.h"
#include "Validator.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

// Helper function to parse city names and populations.
std::map<std::string, unsigned int> parse_cities(std::istream& stream, int n) {
  std::map<std::string, unsigned int> cities;
  unsigned int population;
  std::string name;

  for(int i = 0; i < n; ++i) {
    std::string line;
    if(!std::getline(stream, line)) {
      throw std::runtime_error("Could not read city line.");
    }

    std::istringstream linestream(line);
    if(!(linestream >> population >> name)) {
      throw std::runtime_error("Invalid city line: " + line);
    }

    cities[name] = population;
  }

  return cities;
}

// Helper function to parse factory city names.
std::vector<std::string> parse_factories(std::istream& stream, int n) {
  std::vector<std::string> factories;
  std::string name;

  for(int i = 0; i < n; ++i) {
    std::string line;
    if(!std::getline(stream, line)) {
      throw std::runtime_error("Could not read factory line.");
    }

    std::istringstream linestream(line);
    if(!(linestream >> name)) {
      throw std::runtime_error("Invalid factory line: " + line);
    }

    factories.push_back(name);
  }

  return factories;
}

// Helper function to parse routes.
std::vector<Route> parse_routes(std::istream& stream, int n) {
  std::vector<Route> routes;
  Route r;
  r.id = 0;

  for(int i = 0; i < n; ++i) {
    std::string line;
    if(!std::getline(stream, line)) {
      throw std::runtime_error("Could not read route line.");
    }

    std::istringstream linestream(line);
    if(!(linestream >> r.city1 >> r.city2 >> r.days >> r.load >> r.cost)) {
      throw std::runtime_error("Invalid route line: " + line);
    }

    routes.push_back(r);
    r.id += 1;
  }

  return routes;
}


int main(int argc, char** argv) {
  if(argc != 2) {
    std::cerr << "USAGE: ./vaccinate [data-file]\n";
    std::exit(1);
  }

  // Change this to get fewer logs.
  bool       verbose   = true;
  Scheduler* scheduler = nullptr;

  try {
    std::ifstream file(argv[1]);

    unsigned int ndays, estimate, ncities, nfactories, nroutes;
    if(!(file >> ndays >> estimate >> ncities >> nfactories >> nroutes >> std::ws)) {
      throw std::runtime_error("Could not read metadata line.");
    }

    auto cities    = parse_cities(    file, ncities);
    auto factories = parse_factories( file, nfactories);
    auto routes    = parse_routes(    file, nroutes);

    Validator validator(ndays, estimate, cities, factories, routes, verbose);
    scheduler = Scheduler::create(ndays, cities, factories, routes);

    auto schedule = scheduler->schedule();
    validator.validate(schedule);
    validator.summarize();
  }
  catch(const validation_error& e) {
    std::cerr << "Invalid: " << e.what() << "\n";
  }
  catch(const std::exception& e) {
    std::cerr << "Unexpected exception: " << e.what() << "\n";
  }

  delete scheduler;
  return 0;
}
