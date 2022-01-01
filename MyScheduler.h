#ifndef MYSCHEDULER_H
#define MYSCHEDULER_H

#include "Scheduler.h"
#include <queue>
#include <iostream>
#include <set>
#include <utility>
struct City{
	std::string name;
	City* prevCity;
	int routeLength=100000;
	int routeCost=1000000000;
	int id;
	int doses;
	unsigned int population;
	bool visited;
	std::vector<Route*> routes;
};
struct MyRoute{
	City* origin;
	City* end;
	int routeLength;
	int routeCost;
	int id;     // The unique ID of this route.
	int days;   // The number of days it takes to travel this route.
	int load;   // The number of doses that fit in one vehicle.
	int cost;
};

class MyScheduler: public Scheduler {

	struct RouteAscending{

		bool operator()(const MyRoute* lhs,const MyRoute* rhs){
			if(lhs->routeLength!=rhs->routeLength){
				return lhs->routeLength>rhs->routeLength;
			}
			if(lhs->routeCost!=rhs->routeCost)
				return lhs->routeCost>rhs->routeCost;
			return lhs->origin->name>rhs->origin->name;
		}
	};

	// Member Variables
	int deadline;
	//cities with no factories
	std::vector<City*> nfCities; //name,population
	//cities with factories
	std::vector<City*> fCities;
	//all cities
	std::map<std::string,City*> cities;
	std::map<std::string,City*> unfoundCities;
	std::vector<Route*>                  allRoutes;
	std::vector<MyRoute*> myRoutes;
	std::map<std::pair<City*,City*>,MyRoute*> dijkstraRoutes;

	std::priority_queue <MyRoute*,std::vector<MyRoute*>,RouteAscending> unused;

	int numCities;

public:
	// Constructor
	MyScheduler(unsigned int deadline,
			std::map<std::string, unsigned int> cities, //name,population
			std::vector<std::string>            factories,
			std::vector<Route>                  routes
	);
	// Destructor?
	~MyScheduler(){
		for(auto c:cities)
			delete c.second;
		for(Route* r:allRoutes)
			delete r;
		for(MyRoute* r:myRoutes){
			delete r;
		}
	}

	// Required Member Function
	std::vector<Shipment> schedule();
	//returns the length of the route between 2 cities

	int routeLength(City* origin,Route* r);

	int routeCost(City* city);

	int routeCost(City* origin,Route* r);

	void dijkstra();

	void fastestRoutes();
	void fastestRoutes(City* city);

	void insertRoute(City* origin,Route* r);

	City* getCity(std::string name){
		return cities.find(name)->second;
	}
	void resetVisits(){
		for(City* c:nfCities){
			c->visited=false;
		}
	}
};

#endif
