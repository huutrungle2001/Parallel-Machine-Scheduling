#ifndef INSTANCE_H
#define INSTANCE_H
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Instance {
   public:
    int numMachine;
    int numJob;
    vector<double> times;

    Instance(int NM, int NJ, vector<double> t) {
        numMachine = NM;
        numJob = NJ;
        times = t;
    }

    double getTime(int id) {
        return times[id];
    }
};

#endif  // INSTANCE_H
