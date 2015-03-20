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
//// Helper function definitions ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int indexAbove(std::vector<double>, double time);
std::vector<std::vector<double> > insert(std::vector<std::vector<double> > vec,
                                         int index,
                                         std::vector<double>);
std::vector<double> interpolate(std::vector<double> times,
                                std::vector<std::vector<double> > values,
                                double time);


////////////////////////////////////////////////////////////////////////////////
//// Process member function definitions ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
Process::Process(std::vector<double> initial_pos){
    times.push_back(0.0);
    values.push_back(initial_pos);
    dimension = initial_pos.size();
};

void Process::setValue(double time, std::vector<double> position){
    auto elem = std::max_element(times.begin(), times.end());
    if (time > *elem) {
        times.push_back(time);
        values.push_back(position);
    }
    else {
        int index = indexAbove(times, time);
        times.insert(times.begin()+index, time);
        values.insert(values.begin()+index, position);
    }
};

std::vector<std::vector<double> > Process::getValues(){
    return values;
};

std::vector<double> Process::getTimes(){
    return times;
};


////////////////////////////////////////////////////////////////////////////////
//// Stochastic Process member function definitions ////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::vector<double> StochasticProcess::operator()(double time){
    std::vector<double> val;
    val = interpolate(times, values, time);
    return val;
};


////////////////////////////////////////////////////////////////////////////////
//// Brownian motion member function definitions ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////
std::vector<double> BrownianMotion::getValue(double time){
    std::vector<double> val;
    double change = 0;
    if (time > times.back()) {
        double dt = time - times.back();
        for (int i = 0; i < dimension; i++) {
            change = rnorm(generator)*sqrt(dt);
            val.push_back(values.back()[i] + change);
        }
    }
    else {
        int ind = indexAbove(times, time);
        std::vector<double> interp = interpolate(times, values, time);
        double dt = times[ind] - times[ind-1];
        double conditionalVariance =
            (time - times[ind-1])*(times[ind] - time)/dt;
        for (int i = 0; i < dimension; i++) {
            change = rnorm(generator)*sqrt(conditionalVariance);
            val.push_back(interp[i] + change);
        }
    }
    setValue(time, val);
    return val;
};

////////////////////////////////////////////////////////////////////////////////
//// Helper function implementations ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Finds the index a time should be inserted into a sorted vector of times
int indexAbove(std::vector<double> sortedVec, double time){
    int index = 0;
    while (sortedVec.at(index) < time) {
        index++;
    }
    return index;
};

// Linearly interpolates between values to give approximate value at time
std::vector<double> interpolate(std::vector<double> times,
                                std::vector<std::vector<double> > values,
                                double time){
    std::vector<double> val(values[0].size());
    int ind;
    ind = indexAbove(times, time);
    std::vector<double> lval = values[ind-1];
    std::vector<double> rval = values[ind];
    double ltime = times[ind-1];
    double rtime = times[ind];
    double difference = rtime - ltime;
    for (int i = 0; i < lval.size(); i++) {
        val[i] = ( lval[i]*(rtime - time) + rval[i]*(time - ltime) )/difference;
    }
    return val;
};









