//
//  sle_process.cpp
//  SLE
//
//  Created by Henry Jackson on 20/03/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#include "sle_process.h"
#include <cmath>

StochasticProcess SLE(double kappa, double t_end, double dt){
    std::vector<double> init(2, 0.0);
    StochasticProcess sle(init);
    
    return sle;
};

std::complex<double> SlitMap::operator()(std::complex<double> z){
    std::complex<double> lh = z + 2*sqrt(dt*(1-alpha)/alpha);
    std::complex<double> rh = z - 2*sqrt(dt*alpha/(1-alpha));
    return pow(lh, 1-alpha)*pow(rh, alpha);
};