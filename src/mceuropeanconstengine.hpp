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
#include "./blackscholesconstprocess.hpp"
#include <iostream>
using namespace std;

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
                 seed),
                 realProcess(process),
                 seed_(seed),
                 brownianBridge_(brownianBridge),
                 ifConst(true){};   
        void setConst(bool isConst){ifConst = isConst;};
     protected:
            boost::shared_ptr<path_generator_type> pathGenerator() const {
                if(ifConst){
                    boost::shared_ptr<BlackScholesConstProcess> constProcess_(
                    new BlackScholesConstProcess(
                        realProcess->stateVariable(),
                        realProcess->dividendYield(),
                        realProcess->riskFreeRate(),
                        realProcess->blackVolatility()                                              
                    ));
                    
                    Size dimensions = constProcess_->factors();
                    TimeGrid grid = this->timeGrid();
                    typename RNG::rsg_type generator =
                        RNG::make_sequence_generator(dimensions*(grid.size()-1),seed_);
                    cout<<"this is a const result"<<endl;
                    return boost::shared_ptr<path_generator_type>(
                            new path_generator_type(constProcess_, grid,
                                           generator, brownianBridge_));
                    
                }else{
                    return MCEuropeanEngine<RNG,S>::pathGenerator();
                }
            };
            bool ifConst; 
            boost::shared_ptr<GeneralizedBlackScholesProcess> realProcess;      
            bool brownianBridge_;
            BigNatural seed_;      
    };

    //! Monte Carlo European engine factory
    template <class RNG = PseudoRandom, class S = Statistics>
    class MakeMCEuropeanConstEngine {
      public:
        MakeMCEuropeanConstEngine(
                    const boost::shared_ptr<GeneralizedBlackScholesProcess>&);
        // named parameters
        MakeMCEuropeanConstEngine& withSteps(Size steps);
        MakeMCEuropeanConstEngine& withStepsPerYear(Size steps);
        MakeMCEuropeanConstEngine& withBrownianBridge(bool b = true);
        MakeMCEuropeanConstEngine& withSamples(Size samples);
        MakeMCEuropeanConstEngine& withAbsoluteTolerance(Real tolerance);
        MakeMCEuropeanConstEngine& withMaxSamples(Size samples);
        MakeMCEuropeanConstEngine& withSeed(BigNatural seed);
        MakeMCEuropeanConstEngine& withAntitheticVariate(bool b = true);
        // conversion to pricing engine
        operator boost::shared_ptr<PricingEngine>() const;
      private:
        boost::shared_ptr<GeneralizedBlackScholesProcess> process_;
        bool antithetic_;
        Size steps_, stepsPerYear_, samples_, maxSamples_;
        Real tolerance_;
        bool brownianBridge_;
        BigNatural seed_;
    };

    template <class RNG, class S>
    inline MakeMCEuropeanConstEngine<RNG,S>::MakeMCEuropeanConstEngine(
             const boost::shared_ptr<GeneralizedBlackScholesProcess>& process)
    : process_(process), antithetic_(false),
      steps_(Null<Size>()), stepsPerYear_(Null<Size>()),
      samples_(Null<Size>()), maxSamples_(Null<Size>()),
      tolerance_(Null<Real>()), brownianBridge_(false), seed_(0) {}

    template <class RNG, class S>
    inline MakeMCEuropeanConstEngine<RNG,S>&
    MakeMCEuropeanConstEngine<RNG,S>::withSteps(Size steps) {
        steps_ = steps;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCEuropeanConstEngine<RNG,S>&
    MakeMCEuropeanConstEngine<RNG,S>::withStepsPerYear(Size steps) {
        stepsPerYear_ = steps;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCEuropeanConstEngine<RNG,S>&
    MakeMCEuropeanConstEngine<RNG,S>::withSamples(Size samples) {
        QL_REQUIRE(tolerance_ == Null<Real>(),
                   "tolerance already set");
        samples_ = samples;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCEuropeanConstEngine<RNG,S>&
    MakeMCEuropeanConstEngine<RNG,S>::withAbsoluteTolerance(Real tolerance) {
        QL_REQUIRE(samples_ == Null<Size>(),
                   "number of samples already set");
        QL_REQUIRE(RNG::allowsErrorEstimate,
                   "chosen random generator policy "
                   "does not allow an error estimate");
        tolerance_ = tolerance;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCEuropeanConstEngine<RNG,S>&
    MakeMCEuropeanConstEngine<RNG,S>::withMaxSamples(Size samples) {
        maxSamples_ = samples;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCEuropeanConstEngine<RNG,S>&
    MakeMCEuropeanConstEngine<RNG,S>::withSeed(BigNatural seed) {
        seed_ = seed;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCEuropeanConstEngine<RNG,S>&
    MakeMCEuropeanConstEngine<RNG,S>::withBrownianBridge(bool brownianBridge) {
        brownianBridge_ = brownianBridge;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCEuropeanConstEngine<RNG,S>&
    MakeMCEuropeanConstEngine<RNG,S>::withAntitheticVariate(bool b) {
        antithetic_ = b;
        return *this;
    }

    template <class RNG, class S>
    inline
    MakeMCEuropeanConstEngine<RNG,S>::operator boost::shared_ptr<PricingEngine>()
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
