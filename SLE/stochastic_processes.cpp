//
//  stochastic_processes.cpp
//  SLE
//
//  Created by Henry Jackson on 18/03/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#include "stochastic_processes.h"

////////////////////////////////////////////////////////////////////////////////
//// Helper function definitions ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int indexAbove(std::vector<double>, double time);
std::vector<std::vector<double> > insert(std::vector<std::vector<double> > vec,
                                         int index,
                                         std::vector<double>);


////////////////////////////////////////////////////////////////////////////////
//// Process member function definitions ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
Process::Process(std::vector<double> initial_pos){
    std::vector<double> times(1, 0.0);
    std::vector<std::vector<double> > values;
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