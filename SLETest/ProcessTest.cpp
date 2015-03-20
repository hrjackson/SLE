//
//  ProcessTest.cpp
//  SLE
//
//  Created by Henry Jackson on 19/03/2015.
//  Copyright (c) 2015 n/a. All rights reserved.
//

#include "catch.hpp"

#include "stochastic_processes.h"

TEST_CASE( "Process class tested", "[Process]"){
    std::vector<double> init(2, 0.0);
    Process proc(init);
    
    SECTION( "Testing initialisation" ){
        REQUIRE( proc.getTimes().size() == 1 );
        REQUIRE( proc.getTimes()[0] == 0.0);
        REQUIRE( proc.getValues().size() == 1);
        REQUIRE( proc.getValues()[0] == init);
    }
    
    SECTION( "Testing setter" ){
        // First, see if it can append a position to the end
        std::vector<double> pos_end(2, 1.0);
        proc.setValue(1.0, pos_end);
        
        // What the times should be
        std::vector<double> t_two(1, 0.0);
        t_two.push_back(1.0);
        
        // What the values should be
        std::vector<std::vector<double> > v_two;
        v_two.push_back(init);
        v_two.push_back(pos_end);
        
        REQUIRE( proc.getTimes() == t_two);
        REQUIRE( proc.getValues() == v_two );
        
        // Next, see if it can place one in the middle as well
        std::vector<double> pos_mid;
        pos_mid.push_back(0.4);
        pos_mid.push_back(0.3);
        proc.setValue(0.6, pos_mid);
        
        // What the times should be
        std::vector<double> t_three(1, 0.0);
        t_three.push_back(0.6);
        t_three.push_back(1.0);
        
        // What the values should be
        std::vector<std::vector<double> > v_three;
        v_three.push_back(init);
        v_three.push_back(pos_mid);
        v_three.push_back(pos_end);
        
        REQUIRE( proc.getTimes() == t_three);
        REQUIRE( proc.getValues() == v_three);
    }
}

TEST_CASE( "StochasticProcess class tested", "[StochasticProcess]"){
    std::vector<double> init(2, 0.0);
    StochasticProcess proc(init);
    
    SECTION( "Testing initialisation" ) {
        REQUIRE( proc.getTimes().size() == 1 );
        REQUIRE( proc.getTimes()[0] == 0.0);
        REQUIRE( proc.getValues().size() == 1);
        REQUIRE( proc.getValues()[0] == init);
    }
    
    SECTION( "Testing interpolation") {
        std::vector<double> pos_end(1, 1.0);
        pos_end.push_back(2.0);
        proc.setValue(1.0, pos_end);
        
        // Should take value at time 0.6
        std::vector<double> pos_mid(1, 0.6);
        pos_mid.push_back(1.2);
        
        REQUIRE (proc(0.6) == pos_mid);
    }
}

TEST_CASE( "BrownianMotion class tested", "[BrownianMotion]"){
    std::vector<double> init(2, 0.0);
    BrownianMotion B(init);
    
    SECTION( "Testing initialisation" ) {
        REQUIRE( B.getTimes().size() == 1 );
        REQUIRE( B.getTimes()[0] == 0.0);
        REQUIRE( B.getValues().size() == 1);
        REQUIRE( B.getValues()[0] == init);
    }
    
    SECTION( "Testing value generation") {
        double t = 1.0;
        std::vector<double> val = B(t);
        REQUIRE( B.getValues().back() == val);
        
        t = 0.4;
        std::vector<double> mid_val = B(t);
        std::vector<double> times(1, 0.0);
        times.push_back(0.4);
        times.push_back(1.0);
        std::vector<std::vector<double> > values;
        values.push_back(init);
        values.push_back(mid_val);
        values.push_back(val);
        
        REQUIRE( B.getTimes() == times);
        REQUIRE( B.getValues() == values);
    }
}
























