//
//  SLETest.cpp
//  SLE
//
//  Created by Henry Jackson on 23/03/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#include "catch.hpp"

#include "sle_process.h"

TEST_CASE( "SlitMap class tested", "[SlitMap]"){
    SlitMap id(0.5, 0);
    double dt = 0.1;
    double alpha = 0.4;
    SlitMap h(alpha, dt);
    double eps = 0.00000001;
    std::complex<double> z(1,1);
    std::complex<double> w(2,3);
    std::complex<double> zero(0,0);
    
    SECTION( "Testing Identity" ){
        REQUIRE( abs(id(z) - z) < eps );
        REQUIRE( abs(id(w) - w) < eps );
        REQUIRE( abs(id(zero) - zero) < eps );
    }
    
    SECTION( "Testing general map by hand"){
        std::complex<double> lh;
        std::complex<double> rh;
        std::complex<double> image;
        
        
        lh = z + 2*sqrt(dt*(1-alpha)/alpha);
        rh = z - 2*sqrt(dt*alpha/(1-alpha));
        image = pow(lh, 1-alpha)*pow(rh, alpha);
        
        REQUIRE( abs( h(z) - image) < eps);
        
        lh = w + 2*sqrt(dt*(1-alpha)/alpha);
        rh = w - 2*sqrt(dt*alpha/(1-alpha));
        image = pow(lh, 1-alpha)*pow(rh, alpha);
        
        REQUIRE( abs( h(w) - image) < eps);
        
        lh = zero + 2*sqrt(dt*(1-alpha)/alpha);
        rh = zero - 2*sqrt(dt*alpha/(1-alpha));
        image = pow(lh, 1-alpha)*pow(rh, alpha);
        
        REQUIRE( abs( h(zero) - image) < eps);
        
    }
}

TEST_CASE( "SLE class tested", "[SLE]"){
    std::vector<double> init(1, 0.0);
    BrownianMotion B(init);
    double kappa = 2;
    double t_end = 1;
    double tolerance = 0.1;
    double t_min = 0;
    SLE g(&B, kappa, t_end, tolerance, t_min);
    
    REQUIRE( g.getTimes() == g.getTimesFromZ() );
    REQUIRE( g.getTimes().back() > t_end );
    
}

