# Vaccinate

_Due Saturday, August 29th, before midnight Pacific Time._

**Note:**  You can work on this assignment with a partner if you want to. If you
do,  the two of you should make  one submission  on Gradescope with both of your
names listed.  You'll need to do this every time you submit.


## Backstory

Your biochemical firm  has finally developed a vaccine  that's effective against
the latest  zombie plague.  You spent the last week  mass producing the vaccine,
and you  now have  enough for the  entire  country.  Now it's  just a  matter of
getting the vaccine out of your factories and to the hospitals that need it.

In order to prevent a run on any particular hospital, the government has decided
that the vaccine should be made available in all cities simultaneously. That day
is coming soon, and you'll need to ship your vaccine in advance.  So you call up
a friend at a shipping company and start to figure out a plan...


## Your Assignment

- Create a distribution plan for the zombie vaccine.
- Make sure that every city gets at least as many doses as it needs.
- Spend as little money on shipping as possible.
- You can use anything from the standard library.
- Make sure you don't have any memory leaks.


## What You Know

In addition to how many days you have before the deadline, you know the following
information:

### Cities

The government has given you  a list of cities and the number of doses that will
be required at each city by the deadline.

### Factories

You know which cities your factories are located in.  Your factories can produce
as much vaccine as you need,  and cities containing factories have their vaccine
requirements met automatically.

### Routes

Your friend at the shipping company has given you a list of all available routes
you could use to ship your vaccine.  Each route has the following attributes:

- The **ID** of the route (a unique non-negative integer).
- The names of the two **cities** that are connected by the route.
- The number of **days** it takes to transfer goods along this route.
- The number of doses in one **load** (truck, boat, zeppelin, etc.).
- The **cost** of sending a single load.

Routes can be used in either direction.  You can send a partial load, but you'll
have to pay the full cost for it;  you need the entire truck and driver, even if
the truck isn't fully loaded.

There are no routes with a zero-day shipping time.  A route that takes "one day"
has overnight shipping.  See the Timings section for details.


## What You Want

You need to come up with a distribution plan that gets vaccines to everyone by
the deadline.  You do this by creating a list of the shipments you plan to make.
Each shipment needs the following details specified:

- The **ID** of the route you'll be using.
- The **source** city that the shipment leaves from.
- The **day** that the shipment will leave the source city.
- The number of **doses** you want to ship.

You can ship multiple loads in one shipment. To do this, simply specify a number
of doses that's higher than the route's load size.

The first day of your plan is day zero.


## Timings

Shipments always arrive in the morning and leave in the afternoon. Shipments can
arrive  the morning  of the deadline.  If you have  two days  to get vaccines to
three cities that are connected A-B-C with one-day routes, this is a valid plan:

```
Day 0 (morning): Go!
Day 0 (afternoon):
 - 200 doses leave Aberdeen for Birmingham.
Day 1 (morning):
 - 200 doses arrive in Birmingham.
Day 1 (afternoon):
 - 100 doses leave Birmingham for Canterbury.
Day 2 (morning):
 - 100 doses arrive in Canterbury.
Day 2 (afternoon): Deadline!
```


## Extra Credit

To get full credit, get enough vaccine to all cities before the deadline. To get
extra credit,  save as much money  as you can!  The best  submission will get 20
extra points, the next best will get 18, and so on.
