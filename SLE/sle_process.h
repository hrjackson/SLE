//
//  sle_process.h
//  SLE
//
//  Created by Henry Jackson on 20/03/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#ifndef __SLE__sle_process__
#define __SLE__sle_process__

#include <stdio.h>
#include <vector>
#include <complex>
#include <map>
#include "stochastic_processes.h"

class SLE : public Process {
private:
    BrownianMotion B;
    double kappa;
public:
    double operator()(double time, std::vector<double> point);
    std::vector<double> curve(double time);
};

class SlitMap {
private:
    double alpha;
    double dt;
public:
    std::complex<double> operator()(std::complex<double> z);
};



#endif /* defined(__SLE__sle_process__) */
