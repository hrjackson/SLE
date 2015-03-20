//
//  stochastic_processes.h
//  SLE
//
//  Created by Henry Jackson on 18/03/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#ifndef __SLE__stochastic_processes__
#define __SLE__stochastic_processes__

#include <stdio.h>
#include <vector>
#include <random>

class Process {
protected:
    std::vector<double> times;
    std::vector<std::vector<double> > values;
    unsigned long dimension;
public:
    Process(std::vector<double> initial_pos);
    void setValue(double time, std::vector<double> position);
    std::vector<std::vector<double> > getValues();
    std::vector<double> getTimes();
};

class StochasticProcess : public Process {
public:
    std::vector<double> getValue(double time);
    StochasticProcess(std::vector<double> initial_pos)
    :Process(initial_pos){
        // Write something...
    }
};

class BrownianMotion : public Process {
private:
    std::mt19937_64 generator;
    std::normal_distribution<double> rnorm;
public:
    std::vector<double> getValue(double time);
    BrownianMotion(std::vector<double> initial_pos)
    :Process(initial_pos){
        long long seed = std::chrono::system_clock::now().time_since_epoch().count();
        generator.seed(seed);
    };
};

#endif /* defined(__SLE__stochastic_processes__) */