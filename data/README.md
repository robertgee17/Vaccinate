# Data Files

These are some scenarios to help you test your solution. They're mostly smaller,
simpler versions of the scenarios on Gradescope  -  they're intended to be small
enough to draw out on paper to help with debugging.

- **Freebie**:  There's no need to do anything.  There's only one city,  and you
  have a factory in it.

- **Linear**:  The three-city example from the readme.

- **Chessboard**:  A square grid. Routes connect orthogonally adjacent cities.

- **Clique**:  A fully connected cluster of cities.

- **Hub & Spoke**:  A cluster of large cities that are fully connected by large,
  slow routes.  Each main city also has routes to smaller cities nearby.

- **Network**:  A randomly generated graph.  Each city that's added will have at
  least one route to some previously existing city.

- **Islands**:  Multiple unconnected clusters, as in Networks (above).

- **Ringworld**:  Fast transit along the main ring road; a few outlying areas.


## File Format

All the data files start with the following integer fields on the first line:

- `ndays` is the number of days until the deadline.
- `estimate` is the projected cost of supplying all cities.
- `ncities` is the number of cities you need to supply.
- `nfactories` is the number of factories you have.
- `nroutes` is the number of routes between cities.

The next `ncities` lines describe the cities.  Each line contains:

- `population` is the number of vaccines required in that city.
- `name` is the name of the city.

The next `nfactories` lines  contain one city name each.  These are the names of
the cities which contain your factories.

The final `nroutes` lines describe the routes.  Each line contains:

- `city1` is the name of one city connected by this route.
- `city2` is the name of the other city connected by this route.
- `days` is the number of days (nights) it takes for goods to travel this route.
- `load` is the maximum number of vaccines you can fit in one vehicle.
- `cost` is the price you have to pay per vehicle dispatched.



## Licensing

Tests using real-world city names  use the Simple Maps  [World Cities Database](
https://simplemaps.com/data/world-cities), which is licensed under the [Creative
Commons Attribution 4.0](https://creativecommons.org/licenses/by/4.0/)  license,
as a source for city names.  The version included here has been stripped down to
save space; the original is available at the link above.
