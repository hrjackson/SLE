//
//  sle_process.cpp
//  SLE
//
//  Created by Henry Jackson on 20/03/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#include "sle_process.h"
#include <cmath>
#include <iostream>
#include <algorithm>


/*//////////////////////////////////////////////////////////////////////////////
//// SlitMap private member functions //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////*/

std::complex<double> SlitMap::derivative(std::complex<double> z) {
    std::complex<double> lhQ = z + lhQuotient;
    std::complex<double> rhQ = z - rhQuotient;
    return (1 - alpha + lhQ/rhQ)*pow(rhQ/lhQ, alpha);
}

/*//////////////////////////////////////////////////////////////////////////////
//// SlitMap member functions //////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////*/

SlitMap::SlitMap(double alpha, double dt)
:alpha(alpha), dt(dt) {
    oneMinusAlpha = 1-alpha;
    lhQuotient = 2*sqrt(dt*oneMinusAlpha/alpha);
    rhQuotient = 2*sqrt(dt*alpha/oneMinusAlpha);
    tol = 0.0000001;
};

SlitMap::SlitMap(){
    alpha = 0.5;
    dt = 0.5;
    oneMinusAlpha = 0.5;
    lhQuotient = 1;
    rhQuotient = 1;
    tol = 0.000001;
    std::cout << "REALLY shouldn't be needing the default constructor!" << std::endl;
}

std::complex<double> SlitMap::operator()(std::complex<double> z){
    // Old code:
    //std::complex<double> lh = z + 2*sqrt(dt*(1-alpha)/alpha);
    std::complex<double> lhQ = z + lhQuotient;
    //std::complex<double> rh = z - 2*sqrt(dt*alpha/(1-alpha));
    std::complex<double> rhQ = z - rhQuotient;
    return pow(lhQ, 1-alpha)*pow(rhQ, alpha);
    //Optimised:
    //return pow(z + lhQuotient, oneMinusAlpha)*pow(z + rhQuotient, alpha);
}

// Simple Newton-Raphson calculation. 
std::complex<double> SlitMap::inverse(std::complex<double> w){
    std::complex<double> z(0,5);
    std::complex<double> image = this->operator()(z);
    int i = 0;
    while ( (abs(image - w) > tol) && i < 1000 ) {
        z = z - (image - w)/derivative(z);
        image = this->operator()(z);
        ++i;
    }
    return z;
}

void SlitMap::update(double dt, double alpha){
    this->alpha = alpha;
    this->dt = dt;
    oneMinusAlpha = 1-alpha;
    lhQuotient = 2*sqrt(dt*oneMinusAlpha/alpha);
    rhQuotient = 2*sqrt(dt*alpha/oneMinusAlpha);
}

void SlitMap::setAlpha(double newAlpha){
    alpha = newAlpha;
    oneMinusAlpha = 1-newAlpha;
    lhQuotient = 2*sqrt(dt*oneMinusAlpha/alpha);
    rhQuotient = 2*sqrt(dt*alpha/oneMinusAlpha);
}

void SlitMap::setDt(double newDt){
    dt = newDt;
    lhQuotient = 2*sqrt(dt*oneMinusAlpha/alpha);
    rhQuotient = 2*sqrt(dt*alpha/oneMinusAlpha);
}

double SlitMap::getAlpha(){
    return alpha;
}

double SlitMap::getDt(){
    return dt;
}



////////////////////////////////////////////////////////////////////////////////
//// SLE private member functions //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

double SLE::angle(double t, double tOld){
    double dx = ((*b)(t)[0] - (*b)(tOld)[0])*sqrt(kappa);
    double dt = t - tOld;
    double alpha;
    double v = pow(dx, 2)/dt;
    if (dx > 0) {
        alpha = 0.5 - sqrt(v/(v+16))/2;
    } else {
        alpha = 0.5 + sqrt(v/(v+16))/2;
    }
    return alpha;
};

std::complex<double> SLE::pointEval(std::complex<double> z){
    std::complex<double> result = z;
    for (auto rit = h.rbegin(); rit!= h.rend(); rit++) {
        result = (rit->second)(result);
    }
    return result;
}

void SLE::singleUpdate(double dt,
                       double& t,
                       SlitMap& candH,
                       std::complex<double>& candZ,
                       double& moved,
                       double& slitSize){
    double alpha = angle(t+dt, t);
    candH.update(dt, alpha);
    candZ = pointEval(candH(0));
    moved = abs(candZ - z[t]);
    slitSize = abs( candH(0) );
}

std::vector<double> SLE::findAdmissibleTimes(double t_end){
    double dt = t_end/(numFrames-1);
    std::vector<double> admissibleTimes(1, 0.0);
    for (int i = 1; i < numFrames; ++i) {
        admissibleTimes.push_back(i*dt);
    }
    return admissibleTimes;
}

void SLE::constructProcess(double t_end, double tolerance, double dtMin){
    SlitMap candH(0.5, 0);
    std::complex<double> candZ;
    
    for (auto it = admissibleTimes.begin()+1; it != admissibleTimes.end(); ++it) {
        adaptiveIncrement(*(it-1), *it, tolerance, dtMin, candH, candZ);
    }
}

void SLE::adaptiveIncrement(double t_start,
                            double t_end,
                            double tolerance,
                            double dtMin,
                            SlitMap& candH,
                            std::complex<double>& candZ){
    double t = t_start;
    double dt = 1;
    double moved;
    double slitSize;
    bool ended = false;
    
    while (!ended) {
        // Make sure that we hit the end exactly
        if (t+dt >= t_end) {
            dt = t_end - t;
            ended = true;
        }
        
        // Adaptive loop
        while (true) {
            singleUpdate(dt, t, candH, candZ, moved, slitSize);
            //if ( (moved < tolerance) && (slitSize < 2*tolerance) ) {
            if ( (moved < tolerance) || dt < dtMin) {
                break;
            } else {
                dt = 0.8*dt;
                //dt = std::max(0.8*dt, dtMin);
                ended = false;
            }
        }

        // Make sure there are no rounding errors
        if (ended) {
            t = t_end;
        } else {
            t = t+dt;
        }
        h.insert(std::pair<double, SlitMap>{t, candH});
        z.insert(std::pair<double, std::complex<double>>{t, candZ});
        std::cout << t << std::fixed << std::endl;
        
        // Speed up if we get too slow
        if (moved < tolerance/2) {
            dt = 1.2*dt;
        }
    }
    
}


////////////////////////////////////////////////////////////////////////////////
//// SLE member functions //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

SLE::SLE(BrownianMotion* b,
         double kappa,
         double t_end,
         double tolerance,
         double dtMin,
         int numFrames)
:b(b), kappa(kappa), numFrames(numFrames){
    admissibleTimes = findAdmissibleTimes(t_end);
    
    // Initialise maps
    SlitMap id(0.5, 0);
    h.insert(std::pair<double, SlitMap>(0.0, id));
    z.insert(std::pair<double, std::complex<double>>(0.0, 0.0+0.0i));
    
    constructProcess(t_end, tolerance, dtMin);
}

std::vector<double> SLE::getTimes(){
    std::vector<double> result;
    for (auto it = h.begin(); it!=h.end(); it++) {
        result.push_back(it->first);
    }
    return result;
}

std::set<double> SLE::getOrderedTimes(){
    std::set<double> result;
    for (auto it = h.begin(); it!=h.end(); it++) {
        result.insert(it->first);
    }
    return result;
}

std::vector<double> SLE::FrameTimes(){
    return admissibleTimes;
}

std::set<double> SLE::orderedFrameTimes(){
    std::set<double> result;
    for (auto it = admissibleTimes.begin(); it != admissibleTimes.end(); ++it) {
        result.insert(*it);
    }
    return result;
}

std::vector<double> SLE::getTimesFromZ(){
    std::vector<double> result;
    for (auto it = z.begin(); it!=z.end(); it++) {
        result.push_back(it->first);
    }
    return result;
}

std::vector<std::complex<double>> SLE::getCurve(){
    std::vector<std::complex<double>> result;
    for (auto it = z.begin(); it!=z.end(); it++) {
        result.push_back(it->second);
    }
    return result;
}

std::complex<double> SLE::forwardPoint(double time, std::complex<double> z){
    std::complex<double> result = z;
    for (auto rit = h.rbegin(); rit != h.rend(); ++rit) {
        if ((rit->first) <= time) {
            result = (rit->second)(result);
        }
    }
    return result;
}

std::complex<double> SLE::reversePoint(double start, double time, std::complex<double> z){
    std::complex<double> result = z;
    //double t_end = h.rbegin()->first;
    for (auto it = h.lower_bound(time); it != h.lower_bound(start); --it) {
        result = (it->second)(result);
    }
    return result;
}

std::vector<std::complex<double>> SLE::forwardLine(double time){
    return forwardLine(0.0, time);
}

std::vector<std::complex<double>> SLE::forwardLine(double tStart, double tEnd){
    std::vector<std::complex<double>> result;
    for (auto it = z.lower_bound(tStart); it != z.upper_bound(tEnd); ++it) {
        result.push_back(it->second);
    }
    return result;
}

std::vector<std::complex<double>> SLE::reverseLine(double time){
    std::vector<std::complex<double>> result;
    double t_end = h.rbegin()->first;
    for (auto it = h.lower_bound(t_end - time); it!=h.end(); ++it) {
        result.push_back(reversePoint(t_end - time, it->first, 0));
    }
    return result;
}

SlitMap SLE::slitMap(double time) {
    double alpha = h[time].getAlpha();
    double dt = h[time].getDt();
    SlitMap result(alpha, dt);
    //SlitMap result(0.3, 0.5);
    return result;
}

double SLE::drivingFunction(double time) {
    return (*b)(time)[0]*sqrt(kappa);
}
