//
//  sle_process.cpp
//  SLE
//
//  Created by Henry Jackson on 20/03/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#include "sle_process.h"
#include <vector>

StochasticProcess SLE(double kappa, double t_end, double dt){
    std::vector<double> init(2, 0.0);
    StochasticProcess sle(init);
    
    return sle;
}