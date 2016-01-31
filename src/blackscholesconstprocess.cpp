/* description
   ...
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
using namespace std;

namespace QuantLib {

    BlackScholesConstProcess::BlackScholesConstProcess(
             const Handle<Quote>& x0,
             const Handle<YieldTermStructure>& dividendTS,
             const Handle<YieldTermStructure>& riskFreeTS,
             const Handle<BlackVolTermStructure>& blackVolTS,
             const boost::shared_ptr<discretization>& disc)
    : StochasticProcess1D(disc), x0_(x0), riskFreeRate_(riskFreeTS),
      dividendYield_(dividendTS), blackVolatility_(blackVolTS) {
        riskFreeForward_ = 0;
        dividendForward_ = 0;
        sigma = blackVolatility_->blackVol(0, x0_->value(), true);
        cout<< riskFreeForward() << endl;
        cout<< dividendForward() << endl;
        cout<< sigma << endl;
        sigma = riskFreeForward() - dividendForward() - 0.5 * sigma * sigma;
        cout<< "sigma =" << sigma << endl;
      }

    Real BlackScholesConstProcess::x0() const {
         
        return x0_->value();
    }
    
    // TODO find good forme of drift, forwardrate
    Real BlackScholesConstProcess::drift(Time t, Real x) const {
        //Real sigma = diffusion();
	return sigma;
    }

    // TODO what's different between localVolatility_ and blackVolatility_
    Real BlackScholesConstProcess::diffusion(Time t, Real x) const {
         
        return sigma;
        //return blackVolatility()->value();
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
                                                     
        //localVolatility(); // trigger update if necessary
        // 
        // if (isStrikeIndependent_) {
        //     // in case of a curve we can calculate exact values
        //     Real variance = blackVolatility_->blackVariance(t0 + dt, 0.01) -
        //                     blackVolatility_->blackVariance(t0, 0.01);
        //     Real drift = (riskFreeRate_->forwardRate(t0, t0 + dt, Continuous,
        //                                              NoFrequency, true) -
        //                   dividendYield_->forwardRate(t0, t0 + dt, Continuous,
        //                                               NoFrequency, true)) *
        //         dt - 0.5 * variance;
        //     return x0 * std::exp( std::sqrt(variance) * dw + drift );
        // } else
        // 
        // TODO find out how to use this function
        return x0_->value();
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
         
	    // TODO how to calculate
        return riskFreeForward_;
        // else ... 
    }

    const Rate BlackScholesConstProcess::dividendForward() const {
         
	    // TODO how to calculate
        return dividendForward_;
        // else ...
    }

}
