//
//  stochastic_processes.cpp
//  SLE
//
//  Created by Henry Jackson on 18/03/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#include "stochastic_processes.h"
#include <cmath>


////////////////////////////////////////////////////////////////////////////////
//// Process member function definitions ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Initialise the process with its starting point
Process::Process(std::vector<double> initial_pos){
    //times.push_back(0.0);
    values.insert( std::pair<double, std::vector<double>> (0.0, initial_pos) );
    dimension = initial_pos.size();
    lastTime = 0;
    lastPos = initial_pos;
};

// Set the value of the process to "position" at time "time"
void Process::setValue(double time, std::vector<double> position){
    values.insert( std::pair<double, std::vector<double>> (time, position));
};

// Return all of the values in the process
std::vector<std::vector<double> > Process::getValues(){
    std::vector<std::vector<double>> result;
    for (auto it = values.begin(); it != values.end(); ++it) {
        result.push_back( it->second );
    }
    return result;
};

// Return all of the times in the process
std::vector<double> Process::getTimes(){
    std::vector<double> result;
    for (auto it = values.begin(); it != values.end(); ++it) {
        result.push_back( it->first );
    }
    return result;
};


////////////////////////////////////////////////////////////////////////////////
//// Stochastic Process member function definitions ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Returns the value of the stochastic process at time "time"
// Linear interpolation between defined times
std::vector<double> StochasticProcess::operator()(double time){
    std::vector<double> val;
    val = interpolate(time);
    return val;
};


////////////////////////////////////////////////////////////////////////////////
//// Brownian motion member function definitions ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Simulates the value of the Brownian motion at time "time"
// Brownian bridge if "time" is in the middle of previously observed times
std::vector<double> BrownianMotion::operator()(double time){
    std::vector<double> val(dimension);
    // Deal with bounds error in messy way.
    if (time <= 0) {
        val = values[0];
        return val;
    }
    
    auto it = values.upper_bound(time);
    double dx;
    if (it == values.end() ) {
        double dt = time - lastTime;
        lastTime = time;
        for (int i = 0; i < dimension; i++) {
            dx = rnorm(generator)*sqrt(dt);
            val[i] = lastPos[i] + dx;
        }
        setValue(time, val);
        lastPos = val;
    } else {
        std::vector<double> interp = interpolate(time);
        double rtime = it->first;
        double ltime = (--it)->first;
        double dt = rtime - ltime;
        double conditionalVariance =
        (time - ltime)*(rtime - time)/dt;
        for (int i = 0; i < dimension; i++) {
            dx = rnorm(generator)*sqrt(conditionalVariance);
            val[i] = interp[i] + dx;
        }
        setValue(time, val);
    }
    return val;
};

////////////////////////////////////////////////////////////////////////////////
//// Private function implementations //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Finds the index a time should be inserted into a sorted vector of times
int Process::indexAbove(std::vector<double> sortedVec, double time){
    int index = 0;
    while (sortedVec.at(index) < time) {
        index++;
    }
    return index;
};

// Linearly interpolates between values to give approximate value at time
std::vector<double> Process::interpolate(double time){
    std::vector<double> val(dimension);
    auto it = values.upper_bound(time);
    double rtime = it->first;
    std::vector<double> rval = it->second;
    double ltime = (--it)->first;
    std::vector<double> lval = it->second;
    double difference = rtime - ltime;
    for (int i = 0; i < dimension; ++i) {
        val[i] = ( lval[i]*(rtime - time) + rval[i]*(time - ltime) )/difference;
    }
    return val;
};









