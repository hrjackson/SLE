//
//  main.cpp
//  SLE
//
//  Created by Henry Jackson on 10/02/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#include <iostream>
#include <vector>
#include <complex>
#include <random>

#include "stochastic_processes.h"
#include "sle_process.h"
#include "plot.h"
#include "SLEAnimate.h"

int main(int argc, const char * argv[]) {
    // Parameters
    double kappa = 4;
    double t_end = 1.0;
    double tolerance = 0.005;
    double dtMin = 0;
    int numFrames = 1800;
    
    int width = 2880;
    int height = 1800;
    int scale = height/2;
    double border = 0.03;
    
    double gridRes = 0.001;
    double gridSpacing = 0.025;
    
    
    // Start
    std::mt19937_64 generator;
    generator.seed(5);
    std::vector<double> init(1, 0.0);
    std::vector<double> mid(1, -0.6);
    std::vector<double> end(1, 0.2);
    BrownianMotion B(init, generator);
    
    std::cout.precision(20);
    
    SLE g(&B, kappa, t_end, tolerance, dtMin, numFrames);
    
    plot plotLeft(width, height, scale, border);
    plot plotRight(width, height, scale, border);
    
    std::cout << "Just about to initialise the animation object" << std::endl;
    SLEAnimate sa(gridRes, gridSpacing, g, plotLeft, plotRight);
    std::cout<< "finished that" << std::endl;
    
    for (int i=0; i < numFrames-1; ++i) {
        sa.output(i);
        std::cout << "Frame " << i << std::endl;
        sa.nextFrame();
        //sa.output();
    }
    sa.output(numFrames-1);
    
    return 0;
}
