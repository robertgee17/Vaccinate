#! /usr/bin/env python
import csv
import numpy
import random
import sys

class Route(object):
    def __init__(self, city1, city2, days=1, load=100, cost=100):
        self.city1 = city1
        self.city2 = city2
        self.days  = days
        self.load  = load
        self.cost  = cost

    @staticmethod
    def random(city1, city2, tier=None, min=0, max=15):
        if tier is None:
            tier = random.randrange(min, max)

        if tier < 8:
            load = 100    + random.randrange(11) * 10
            cost = 100    + random.randrange(101)
            return Route(city1, city2, 1, load, cost)
        if tier < 12:
            load = 1000   + random.randrange(11) * 100
            cost = 400    + random.randrange(401)
            return Route(city1, city2, 2, load, cost)
        if tier < 14:
            load = 10000  + random.randrange(11) * 1000
            cost = 1600   + random.randrange(1601)
            return Route(city1, city2, 4, load, cost)
        if tier < 15:
            load = 100000 + random.randrange(11) * 10000
            cost = 6400   + random.randrange(6401)
            return Route(city1, city2, 7, load, cost)

        raise Exception("!!?!?")


def exprand(m=8, s=3, add=543):
    return int(numpy.random.lognormal(m, s)) + add

def load_names(country, n=None):
    names = set()
    with open('etc/cities.tsv') as file:
        reader = csv.reader(file, delimiter='\t')
        for row in reader:
            if row[1] == country:
                names.add(row[0])
    if n is not None:
        names = random.sample(names, n)
    return names

def output(cities, factories, routes):
    print('99\t99\t%d\t%d\t%d' % (
        len(cities),
        len(factories),
        len(routes)
    ))

    for name, population in cities.items():
        print('%d\t%s' % (population, name))
    for name in factories:
        print(name)
    for route in routes:
        print('%s\t%s\t%d\t%d\t%d' % (
            route.city1,
            route.city2,
            route.days,
            route.load,
            route.cost
        ))


def chessboard(n, f):
    grid   = []
    cities = {}
    for i in range(n):
        row = ['%s%d' % (chr(c + 65), i+1) for c in range(n)]
        for name in row:
            cities[name] = exprand()
        grid.append(row)

    factories = random.sample(cities.keys(), f)
    routes    = []
    for x in range(n):
        for y in range(n):
            if x > 0:
                route = Route.random(grid[y][x], grid[y][x-1])
                routes.append(route)
            if y > 0:
                route = Route.random(grid[y][x], grid[y-1][x])
                routes.append(route)

    output(cities, factories, routes)

def clique(n, f):
    names     = load_names('Italy', n)
    cities    = {name:exprand() for name in names}
    factories = random.sample(names, f)
    routes    = []
    for i in range(len(names)):
        for j in range(i):
            route = Route.random(names[i], names[j])
            routes.append(route)
    output(cities, factories, routes)

def hub_and_spoke(h, s, f):
    names  = load_names('United States', h + s)
    hubs   = {name:exprand(12, 2) for name in names[:h]}
    spokes = {name:exprand( 6, 3) for name in names[h:]}

    cities    = dict(**hubs, **spokes)
    factories = random.sample(names, f)
    routes    = []

    for i in range(h):
        for j in range(i):
            route = Route.random(names[i], names[j], min=12)
            routes.append(route)

    for i in range(h, h+s):
        j = random.randrange(h)
        route = Route.random(names[i], names[j], max=12)
        routes.append(route)

    output(cities, factories, routes)

def mesh(names, p=0.3):
    routes = []
    for i in range(1, len(names)):
        loop = True
        while loop:
            j = random.randrange(i)
            routes.append(Route.random(names[i], names[j]))
            loop = (random.random() < p)
    return routes

def network(n, f):
    names     = load_names('Mongolia', n)
    cities    = {name:exprand() for name in names}
    factories = random.sample(names, f)
    routes    = mesh(names)
    output(cities, factories, routes)

def islands(n, c):
    names     = load_names('Philippines', c)
    cities    = {name:exprand() for name in names}
    factories = []
    routes    = []

    start = 0
    for i in range(n):
        end   = int((i + 1.0) / n * len(names))
        local = names[start:end]
        start = end

        routes += mesh(local)
        factories.append(random.choice(local))

    output(cities, factories, routes)

def ringworld(n, c, f):
    names     = load_names('Colombia', n + c)
    cities    = {name:exprand() for name in names}
    factories = random.sample(names, f)
    routes    = []

    for i in range(n):
        routes.append(Route(names[i], names[(i + 1) % n], 1, 1000, 100))
        routes.append(Route(names[i], names[(i + 2) % n], 2, 1000, 100))
        routes.append(Route(names[i], names[(i + 3) % n], 3, 1000, 100))

    for i in range(n, n+c):
        j = random.randrange(n)
        route = Route.random(names[i], names[j])
        routes.append(route)

    output(cities, factories, routes)

# chessboard(4, 2)
# clique(7, 2)
# hub_and_spoke(5, 15, 2)
# network(18, 2)
# islands(3, 27)
ringworld(7, 12, 2)

