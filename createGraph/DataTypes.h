#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <numeric>
#include <string>
#include <cstdlib>
#include <limits>
#include <linux/limits.h>
#include <cstdio>
#include <utility>
#include <unistd.h>

class Node;
class Link;
class Road;
class VDS;

typedef std::map<int, Node*> NodeMap;
typedef std::map<int, Link*> LinkMap;
typedef std::map<int, Road*> RoadMap;
typedef std::map<int, VDS*> VDSMap;

typedef std::vector<std::string> StringVector;

enum Direction{oneway, bidirectional};

const double PI = 3.141592653589793238463;
const double earthRadiusKm = 6371.0;

#endif