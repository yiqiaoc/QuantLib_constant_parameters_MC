/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2016 Jianfei Pan


 This file is part of the QuantLib constant parameters project
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file MCEuropeanConstEngine.hpp
    \brief Monte Carlo European option engine
*/

#ifndef quantlib_montecarlo_european_const_engine_hpp
#define quantlib_montecarlo_european_const_engine_hpp

#include <ql/pricingengines/vanilla/mceuropeanengine.hpp>

namespace QuantLib {

    template <class RNG = PseudoRandom, class S = Statistics>
    class MCEuropeanConstEngine : public MCEuropeanEngine<RNG,S> {
      public:
        typedef
        typename MCVanillaEngine<SingleVariate,RNG,S>::path_generator_type
            path_generator_type;
        typedef
        typename MCVanillaEngine<SingleVariate,RNG,S>::path_pricer_type
            path_pricer_type;
        typedef typename MCVanillaEngine<SingleVariate,RNG,S>::stats_type
            stats_type;
        // constructor
        MCEuropeanConstEngine(
             const boost::shared_ptr<GeneralizedBlackScholesProcess>& process,
             Size timeSteps,
             Size timeStepsPerYear,
             bool brownianBridge,
             bool antitheticVariate,
             Size requiredSamples,
             Real requiredTolerance,
             Size maxSamples,
             BigNatural seed) : MCEuropeanEngine<RNG,S>(
                 process,
                 timeSteps,
                 timeStepsPerYear,
                 brownianBridge,
                 antitheticVariate,
                 requiredSamples,
                 requiredTolerance,
                 maxSamples,
                 seed){ifConst = false;};
        void setConst(bool isConst){ifConst = isConst;};
        protected:
            boost::shared_ptr<path_generator_type> pathGenerator() const {
                    return MCEuropeanEngine<RNG,S>::pathGenerator();
            };
        private:
            bool ifConst;  
    };

    //! Monte Carlo European engine factory
    template <class RNG = PseudoRandom, class S = Statistics>
    class MakeMCEuropeanConstEngine:public MakeMCEuropeanEngine {
      public:
        MakeMCEuropeanConstEngine(
                    const boost::shared_ptr<GeneralizedBlackScholesProcess>& process):MakeMCEuropeanEngine(process){};
        
        operator boost::shared_ptr<PricingEngine>() const;
   
    };

    template <class RNG, class S>
        inline
        MakeMCEuropeanEngine<RNG,S>::operator boost::shared_ptr<PricingEngine>()
                                                                          const {
            QL_REQUIRE(steps_ != Null<Size>() || stepsPerYear_ != Null<Size>(),
                       "number of steps not given");
            QL_REQUIRE(steps_ == Null<Size>() || stepsPerYear_ == Null<Size>(),
                       "number of steps overspecified");
            return boost::shared_ptr<PricingEngine>(new
                MCEuropeanConstEngine<RNG,S>(process_,
                                        steps_,
                                        stepsPerYear_,
                                        brownianBridge_,
                                        antithetic_,
                                        samples_, tolerance_,
                                        maxSamples_,
                                        seed_));
    }

}


#endif
