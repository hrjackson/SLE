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

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


using namespace std;
using namespace cv;

int main(int argc, const char * argv[]) {
    // Parameters
    
	vector<double> kappa = { 2 };
    double t_end = 0.99;
    double tolerance = 0.001;
    double dtMin = 0;
    int numFrames = 2;
    
    int width =  2880;
    int height = 1800;
    int scale = height/2;
    double border = 0.03;
    
    double gridRes = 0.01;
    double gridSpacing = 0.05;
    
    //plot p(width, height, scale, border);
    //cout << p.points() << endl;
    //cout << p.CVTocpx(Point(1,1)) << endl;
    //cout << p.cpxToCV(complex<double>(1.0, 1.0));
    
    
    
    // Start
	BrownianMotion B(vector < double > {0.0});
    
    std::cout.precision(20);
    
	for (auto kap = kappa.begin(); kap != kappa.end(); ++kap)
	{


		SLE g(&B, *kap, t_end, tolerance, dtMin, numFrames);

		plot plotLeft(width, height, scale, border);
		plot plotRight(width, height, scale, border);
		plot plotReverse(width, height, scale, border);

		std::cout << "Just about to initialise the animation object" << std::endl;
		SLEAnimate sa(gridRes, gridSpacing, g, plotLeft, plotRight);
		std::cout << "finished that" << std::endl;
		//sa.plotForward();
		//sa.plotReverse();

		for (int i = 0; i < numFrames - 1; ++i) {
			sa.output(i);
			std::cout << "Frame " << i << std::endl;
			sa.nextFrame();
			//sa.output();
		}
		sa.output(numFrames - 1);
	}
    
    
    return 0;
}
