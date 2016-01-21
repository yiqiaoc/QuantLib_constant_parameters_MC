/* description
   ...
*/

#ifndef quantlib_black_scholes_const_process_hpp
#define quantlib_black_scholes_const_process_hpp

#include <ql/stochasticprocess.hpp>
#include <ql/processes/eulerdiscretization.hpp>
#include <ql/termstructures/yieldtermstructure.hpp>
#include <ql/termstructures/volatility/equityfx/blackvoltermstructure.hpp>
#include <ql/termstructures/volatility/equityfx/localvoltermstructure.hpp>
#include <ql/quote.hpp>

namespace QuantLib {

    class LocalConstantVol;
    class LocalVolCurve;

    class BlackScholesConstProcess : public StochasticProcess1D {
      public:
        BlackScholesConstProcess(
        const Handle<Quote>& x0,
        const Handle<YieldTermStructure>& dividendTS,
        const Handle<YieldTermStructure>& riskFreeTS,
        const Handle<Quote>& blackVolTS,
        // TODO initial here or not
        const boost::shared_ptr<discretization>& d =
                  boost::shared_ptr<discretization>(new EulerDiscretization));
        
        Real x0() const;
        
        Real drift(Time t=0, Real x=0) const;
        Real diffusion(Time t=0, Real x=0) const;

        Real apply(Real x0, Real dx) const;
        
        Real expectation(Time t0, Real x0, Time dt) const;
        Real evolve(Time t0, Real x0, Time dt, Real dw) const;
        
        Time time(const Date&) const;
        
        //void update();
        
        const Handle<Quote>& stateVariable() const;
        const Handle<YieldTermStructure>& dividendYield() const;
        const Handle<YieldTermStructure>& riskFreeRate() const;
        const Handle<Quote>& blackVolatility() const;
	    const Rate riskFreeForward() const;
	    const Rate dividendForward() const;
        //const Handle<LocalVolTermStructure>& localVolatility() const;
        
      private:
        Handle<Quote> x0_;
        Handle<YieldTermStructure> riskFreeRate_, dividendYield_;
        Handle<Quote> blackVolatility_;
        mutable RelinkableHandle<LocalVolTermStructure> localVolatility_;
	    Rate riskFreeForward_;
	    Rate dividendForward_;
        //mutable bool updated_, isStrikeIndependent_;
    };

}


#endif
