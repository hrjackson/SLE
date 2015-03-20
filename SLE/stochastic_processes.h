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

class Process {
protected:
    std::vector<double> times;
    std::vector<std::vector<double> > values;
    unsigned long dimension;
public:
    Process(std::vector<double>);
    void setValue(double time, std::vector<double> position);
    std::vector<std::vector<double> > getValues();
    std::vector<double> getTimes();
};

class StochasticProcess : public Process {
public:
    std::vector<double> getValue(double time);
};

class BrownianMotion : public Process {
public:
    std::vector<double> getValue(double time);
};

#endif /* defined(__SLE__stochastic_processes__) */