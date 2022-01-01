
#include "MyScheduler.h"

Scheduler* Scheduler::create(
		unsigned int                        deadline,
		std::map<std::string, unsigned int> cities,
		std::vector<std::string>            factories,
		std::vector<Route>                  routes
) {
	return new MyScheduler(deadline,cities,factories,routes);
}

// MyScheduler Member Functions
MyScheduler::MyScheduler(unsigned int deadline,
		std::map<std::string, unsigned int> cities, //name,population
		std::vector<std::string>            factories,
		std::vector<Route>                  routes){
	this->deadline=deadline;
	//populate city map with cities
	int id=0;
	for (auto itr : cities){
		City* c=new City;
		c->name=itr.first;
		c->prevCity=nullptr;
		c->population=itr.second;
		c->id=id;
		c->doses=0;
		c->visited=false;
		id++;
		this->cities.insert({c->name,c});
		this->unfoundCities.insert({c->name,c});
		for(std::string f:factories){
			if(c->name==f){
				c->prevCity=c;
				c->visited=true;
				c->routeLength=0;
				c->routeCost=0;
				fCities.push_back(c);
				break;
			}
		}
		if(!c->visited)
			this->nfCities.push_back(c);
	}

	//send all routes into my own vector of routes
	for(Route route:routes){
		Route* r=new Route;
		r->id=route.id;
		r->city1=route.city1;
		r->city2=route.city2;
		r->days=route.days;
		r->load=route.load;
		r->cost=route.cost;
		allRoutes.push_back(r);
	}
	numCities=cities.size();
	//populate matrix with route lengths
	for(Route* r:allRoutes){
		City* city1=getCity(r->city1);
		City* city2=getCity(r->city2);
		city1->routes.push_back(r);
		city2->routes.push_back(r);
	}
}

std::vector<Shipment> MyScheduler::schedule(){
	std::vector<Shipment> shipments;
	dijkstra();
	for(City* c:nfCities){
		City* tempCity=c;
		//insert the necessary doses to all cities in the path of this city
		while(tempCity->prevCity!=tempCity){
			tempCity->doses+=c->population;
			tempCity=tempCity->prevCity;
		}
	}
	for(City* c:nfCities){
		//std::cout<<c->prevCity->name<<" -> "<<c->name<<std::endl;
		//	std::cout<<"Leaving day "<<c->prevCity->routeLength<<": "<<c->doses<<std::endl;

		std::pair<City*,City*> cityPair;
		cityPair.first=c->prevCity;
		cityPair.second=c;
		MyRoute* r=dijkstraRoutes.find(cityPair)->second;
		Shipment s;
		s.route_id=r->id;
		s.source=c->prevCity->name;
		s.doses=c->doses;
		s.day=c->prevCity->routeLength;
		shipments.push_back(s);

	}
	return shipments;
}


void MyScheduler::dijkstra(){
	//add each route to a factory
	for(City* factory:fCities){
		for(Route* r:factory->routes){
			if(!(getCity(r->city1)->visited&&
					getCity(r->city2)->visited)){
				insertRoute(factory,r);
			}
		}
	}
	while(!unused.empty()){
		//std::cout<<"begin loop"<<std::endl;
		//find the fastest route available
		MyRoute* r=unused.top();
		//remove the found route from the queue
		unused.pop();
		while(r->end->visited&&
				!unused.empty()){
			//repeat the above process while both cities have been visited already
			//(a faster route was found already)
			r=unused.top();
			unused.pop();
		}
		if(r->end->visited)
			break;
		//std::cout<<"===insert route==="<<std::endl;
		//std::cout<<r->origin->name<<"->";
		//std::cout<<r->end->name<<" "<<r->routeLength<<std::endl;
		std::pair<City*,City*> cityPair;
		cityPair.first=r->origin;
		cityPair.second=r->end;
		dijkstraRoutes.insert({cityPair,r});
		City* nextCity=r->end;
		City* prevCity=r->origin;
		nextCity->prevCity=prevCity;
		nextCity->routeCost=r->routeCost;
		nextCity->routeLength=r->routeLength;
		nextCity->visited=true;
		//	std::cout<<"searching"<<std::endl;
		for(Route* r:nextCity->routes){
			if(!(getCity(r->city1)->visited&&
					getCity(r->city2)->visited)){
				insertRoute(nextCity,r);
			}
		}
	}
}


void MyScheduler::insertRoute(City* origin,Route* r){

	//std::cout<<"possible route "<<std::endl;
	MyRoute* route=new MyRoute;
	route->origin=origin;
	if(route->origin->name==r->city1)
		route->end=getCity(r->city2);
	else
		route->end=getCity(r->city1);

	//std::cout<<route->origin->name<<"->";
	//std::cout<<route->end->name<<" ";
	route->routeLength=routeLength(origin,r);
	//std::cout<<route->routeLength<<std::endl;
	route->routeCost=origin->routeCost+r->cost;
	route->id=r->id;
	route->days=r->days;
	route->load=r->load;
	route->cost=r->cost;
	myRoutes.push_back(route);
	unused.push(route);
}
int MyScheduler::routeLength(City* origin,Route* r){
	return origin->routeLength+r->days;
}

int MyScheduler::routeCost(City* city){
	return city->routeCost;
}

int MyScheduler::routeCost(City* origin,Route* r){
	return routeCost(origin)+r->days;
}
