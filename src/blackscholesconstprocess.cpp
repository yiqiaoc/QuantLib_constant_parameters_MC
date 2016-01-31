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

        riskFreeForward_ = riskFreeRate_->zeroRate(1, Continuous, NoFrequency, true);
        dividendForward_ = dividendYield_->zeroRate(1, Continuous, NoFrequency, true);
        sigma = blackVolatility_->blackVol(1, x0->value(), true);
        drift_ = riskFreeForward_ - dividendForward_ - 0.5 * sigma * sigma;
        cout << "sigma = " << sigma << endl;
        cout << "riskFreeForward = " << riskFreeForward_ << endl;
        cout << "dividendForward = " << dividendForward_ << endl;
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
        //cout << "diffusion " << t << " " << x << endl;
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
         
        return riskFreeForward_;
    }

    const Rate BlackScholesConstProcess::dividendForward() const {
           // else ...
        return dividendForward_;
    }

}
