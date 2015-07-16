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
#include <set>
#include <complex>
#include <thrust/complex.h>
#include <map>
#include "stochastic_processes.h"

using namespace std;
typedef thrust::complex<double> cpx;

class SlitMap {
private:
    double dt;
    double offset;
    cpx mySqrt(cpx z);
public:
    SlitMap(double dt, double offset);
    SlitMap();
    cpx operator()(cpx z);
    cpx inverse(cpx w);
    void update(double offset, double dt);
    double getOffset();
    double getDt();
};


class SLE {
private:
    /*---- Data ----*/
    BrownianMotion* b;
    double kappa;
    std::map<double, SlitMap> h;
    std::map<double, cpx> z;
    int numFrames;
    // A vector of times which are guaranteed to be hit by adaptive process
    std::vector<double> admissibleTimes;
    /*---- Functions ----*/
    double angle(double t, double tOld);
    cpx pointEval(cpx z);
    void singleUpdate(double dt,
                      double& t,
                      SlitMap& candH,
                      cpx& candZ,
                      double& moved,
                      double& slitSize);
    std::vector<double> findAdmissibleTimes(double t_end);
    void constructProcess(double t_end, double tolerance, double dtMin);
    void adaptiveIncrement(double t_start,
                           double t_end,
                           double tolerance,
                           double dtMin,
                           SlitMap& candH,
                           cpx& candZ);
public:
    SLE(BrownianMotion* b,
        double kappa,
        double t_end,
        double tolerance,
        double dtMin,
        int numFrames);
    double operator()(double time, std::vector<double> point);
	int numMaps();
    std::vector<double> getTimes();
    std::set<double> getOrderedTimes();
    std::vector<double> FrameTimes();
    std::set<double> orderedFrameTimes();
    std::vector<double> getTimesFromZ();
    std::vector<cpx> getCurve();
    cpx forwardPoint(double time, cpx z);
    cpx reversePoint(double start, double time, cpx z);
    std::vector<cpx> forwardLine(double time);
    std::vector<cpx> forwardLine(double tStart, double tEnd);
    std::vector<cpx> reverseLine(double time);
    SlitMap slitMap(double time);
    double drivingFunction(double time);
    // Returns pointers to arrays of times and shifts.
    double* times();
    double* shifts();
};

#endif /* defined(__SLE__sle_process__) */
