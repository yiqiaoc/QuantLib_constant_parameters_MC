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
#include <math.h>
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

        riskFreeForward_ = riskFreeRate_->zeroRate(9, Continuous, NoFrequency, true);
        dividendForward_ = dividendYield_->zeroRate(9, Continuous, NoFrequency, true);
        sigma = blackVolatility_->blackVol(9, x0->value(), true);

        drift_ = riskFreeForward_ - dividendForward_ - 0.5 * sigma * sigma;
        cout << "sigma = " << sigma <<" "<< "riskFreeForward = " << riskFreeForward_ << " " << "dividendForward = " << dividendForward_ << " "<< "drift = " << drift_ << endl;
        
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
        
        //return  apply(x0, discretization_->drift(*this, t0, x0, dt) +
        //                          tdDeviation(t0, x0, dt)  * dw);
        
        // cout<<"dx : " <<  dt*(drift_-(sigma*sigma)/2) <<"dx +dw : "<<discretization_->drift(*this, t0, x0, dt) +
        //                           stdDeviation(t0, x0, dt)*dw << endl;  
        // cout<< "x : " <<  apply(x0, dt*(drift_-(sigma*sigma)/2))<< "x with wd:"<<
        // apply(x0, discretization_->drift(*this, t0, x0, dt) +
        //                           stdDeviation(t0, x0, dt)  * dw) <<endl;
        //cout<< stdDeviation(t0, x0, dt)<<"---"<<0.5 * sigma * sigma<< "dw:("<<dw<<") "<<  discretization_->drift(*this, t0, x0, dt)<<"---"<<drift_*dt<<"dt("<< dt <<")"<< endl;                  
        //http://quantlib.org/reference/class_quant_lib_1_1_generalized_black_scholes_process.html
        //http://quantlib.org/slides/dima-ql-intro-2.pdf
        
        return apply(x0, dt*drift_ + sqrt(dt)*sigma *dw);
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
