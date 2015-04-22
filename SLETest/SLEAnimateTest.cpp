//
//  SLEAnimateTest.cpp
//  SLE
//
//  Created by Henry Jackson on 22/04/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#include "catch.hpp"

#include "SLEAnimate.h"

TEST_CASE( "SLEAnimate tested", "[SLEAnimate]"){
    ////////////////////////////////////////////////////////////////////////
    // Initialise everything else
    
    // Parameters
    double kappa = 2;
    double t_end = 1.0;
    double tolerance = 0.05;
    double dtMin = 0.0000000000001;
    int numFrames = 10;
    double gridRes = 0.1;
    double gridSpacing = 0.5;
    
    int width = 2880;
    int height = 1800;
    int scale = height/2;
    double border = 0.03;
    
    
    // Start
    std::mt19937_64 generator;
    generator.seed(5);
    std::vector<double> init(1, 0.0);
    BrownianMotion B(init, generator);
    
    SLE g(&B, kappa, t_end, tolerance, dtMin, numFrames);
    
    plot leftPlot(width, height, scale, border);
    plot rightPlot(width, height, scale, border);
    
    SLEAnimate animation(gridRes, gridSpacing, g, leftPlot, rightPlot);
    
    SECTION( "Testing initialisation" ) {
        
    }
}