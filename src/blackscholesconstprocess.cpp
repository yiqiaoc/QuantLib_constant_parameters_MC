/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2016 Yiqiao CHEN


 This file is part of the QuantLib constant parameters project
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include "./blackscholesconstprocess.hpp"
#include <ql/termstructures/volatility/equityfx/localvolsurface.hpp>
#include <ql/termstructures/volatility/equityfx/localvolcurve.hpp>
#include <ql/termstructures/volatility/equityfx/localconstantvol.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <ql/time/calendars/nullcalendar.hpp>
#include <ql/time/daycounters/actual365fixed.hpp>

#include <boost/make_shared.hpp>
#include <iostream>
#include <math.h>
using namespace std;


namespace QuantLib {

    BlackScholesConstProcess::BlackScholesConstProcess(
             const Date& exercisedate,
             const Handle<Quote>& x0,
             const Handle<YieldTermStructure>& dividendTS,
             const Handle<YieldTermStructure>& riskFreeTS,
             const Handle<BlackVolTermStructure>& blackVolTS,
             const boost::shared_ptr<discretization>& disc)
    : StochasticProcess1D(disc), x0_(x0), riskFreeRate_(riskFreeTS),
      dividendYield_(dividendTS), blackVolatility_(blackVolTS) {
          
        Time dt = time(exercisedate);
        riskFreeForward_ = riskFreeRate_->zeroRate(dt, Continuous, NoFrequency, true);
        dividendForward_ = dividendYield_->zeroRate(dt, Continuous, NoFrequency, true);
        sigma = blackVolatility_->blackVol(dt, x0->value(), true);
        drift_ = riskFreeForward_ - dividendForward_ - 0.5 * sigma * sigma;
        expectation_ = dt*drift_;
        stdDev_ = sqrt(dt)*sigma ;
        cout << "====const process=====" << endl;
        cout << "dt = " << dt << "  sigma = " << sigma <<" "<< "riskFreeForward = " << riskFreeForward_ << " " << "dividendForward = " << dividendForward_ << " "<< "drift = " << drift_ << endl;
        cout << "======================" << endl;

      }

    Real BlackScholesConstProcess::x0() const {
         
        return x0_->value();
    }
    
    // TODO find good forme of drift, forwardrate
    Real BlackScholesConstProcess::drift(Time t, Real x) const {
        //Real sigma = diffusion();
	    //return riskFreeForward() - dividendForward() - 0.5 * sigma * sigma;
        //cout << "drift " << t << " " << x << endl;
        return drift_;
    }

    Real BlackScholesConstProcess::diffusion(Time t, Real x) const {
        //cout << "diffusion function " << t << " " << x << endl;
        return sigma;
    }

    Real BlackScholesConstProcess::apply(Real x0, Real dx) const { 
        return x0 * std::exp(dx);
    }

    Real BlackScholesConstProcess::expectation(Time,
                                                     Real,
                                                     Time) const {
                                                          
        QL_FAIL("not implemented");
    }

    Real BlackScholesConstProcess::evolve(Time t0, Real x0,
                                                Time dt, Real dw) const {
        //http://quantlib.org/reference/class_quant_lib_1_1_generalized_black_scholes_process.html
        //http://quantlib.org/slides/dima-ql-intro-2.pdf       
        return apply(x0, expectation_ + dw*stdDev_);
    }

    Time BlackScholesConstProcess::time(const Date& d) const {
         
        return riskFreeRate_->dayCounter().yearFraction(
                                           riskFreeRate_->referenceDate(), d);
    }

    const Handle<Quote>&
    BlackScholesConstProcess::stateVariable() const {
         
        return x0_;
    }

    const Handle<YieldTermStructure>&
    BlackScholesConstProcess::dividendYield() const {
         
        return dividendYield_;
    }

    const Handle<YieldTermStructure>&
    BlackScholesConstProcess::riskFreeRate() const {
         
        return riskFreeRate_;
    }

    const Handle<BlackVolTermStructure>&
    BlackScholesConstProcess::blackVolatility() const {
         
        return blackVolatility_;
    }

    const Rate BlackScholesConstProcess::riskFreeForward() const {
         
        return riskFreeForward_;
    }

    const Rate BlackScholesConstProcess::dividendForward() const {
           // else ...
        return dividendForward_;
    }

}
