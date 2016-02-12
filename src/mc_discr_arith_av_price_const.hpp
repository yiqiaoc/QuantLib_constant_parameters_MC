/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl
 Copyright (C) 2003, 2004 Ferdinando Ametrano

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file mc_discr_arith_av_price.hpp
    \brief Monte Carlo engine for discrete arithmetic average price Asian
*/

#ifndef quantlib_mc_discrete_arithmetic_average_price_asian_const_engine_hpp
#define quantlib_mc_discrete_arithmetic_average_price_asian_const_engine_hpp

#include <ql/pricingengines/asian/mc_discr_geom_av_price.hpp>
#include <ql/pricingengines/asian/analytic_discr_geom_av_price.hpp>
#include <ql/exercise.hpp>

namespace QuantLib {

    //!  Monte Carlo pricing engine for discrete arithmetic average price Asian
    /*!  Monte Carlo pricing engine for discrete arithmetic average price
         Asian options. It can use MCDiscreteGeometricAPEngine (Monte Carlo
         discrete arithmetic average price engine) and
         AnalyticDiscreteGeometricAveragePriceAsianEngine (analytic discrete
         arithmetic average price engine) for control variation.

         \ingroup asianengines

         \test the correctness of the returned value is tested by
               reproducing results available in literature.
    */
    template <class RNG = PseudoRandom, class S = Statistics>
    class MCDiscreteArithmeticAPConstEngine
        : public MCDiscreteArithmeticAPEngine<RNG,S> {
      public:
        typedef
        typename MCDiscreteAveragingAsianEngine<RNG,S>::path_generator_type
            path_generator_type;
        typedef
        typename MCDiscreteAveragingAsianEngine<RNG,S>::path_pricer_type
            path_pricer_type;
        typedef typename MCDiscreteAveragingAsianEngine<RNG,S>::stats_type
            stats_type;
        // constructor
        MCDiscreteArithmeticAPConstEngine(
             const boost::shared_ptr<GeneralizedBlackScholesProcess>& process,
             bool brownianBridge,
             bool antitheticVariate,
             bool controlVariate,
             Size requiredSamples,
             Real requiredTolerance,
             Size maxSamples,
             BigNatural seed,
             bool ifConst);

      protected:
        // McSimulation implementation
        boost::shared_ptr<path_generator_type> pathGenerator() const {
            if(ifConst){
                boost::shared_ptr<BlackScholesConstProcess> constProcess_(
                new BlackScholesConstProcess(
                    process_->stateVariable(),
                    process_->dividendYield(),
                    process_->riskFreeRate(),
                    process_->blackVolatility()                                              
                ));
                
                Size dimensions = process_->factors();
                TimeGrid grid = this->timeGrid();
                typename RNG::rsg_type gen =
                RNG::make_sequence_generator(grid.size()-1,seed_);
                cout<<"this is a const result"<<endl;
                return boost::shared_ptr<path_generator_type>(
                        new path_generator_type(constProcess_, grid,
                                       gen, brownianBridge_));
                    
            }else{
                return MCDiscreteArithmeticAPEngine<RNG,S>::pathGenerator();
            }
        }

      private:
        bool ifconst;
    };


    // inline definitions

    template <class RNG, class S>
    inline
    MCDiscreteArithmeticAPConstEngine<RNG,S>::MCDiscreteArithmeticAPConstEngine(
             const boost::shared_ptr<GeneralizedBlackScholesProcess>& process,
             bool brownianBridge,
             bool antitheticVariate,
             bool controlVariate,
             Size requiredSamples,
             Real requiredTolerance,
             Size maxSamples,
             BigNatural seed,
             bool ifConst)
    : MCDiscreteAveragingAsianEngine<RNG,S>(process,
                                            brownianBridge,
                                            antitheticVariate,
                                            controlVariate,
                                            requiredSamples,
                                            requiredTolerance,
                                            maxSamples,
                                            seed), ifConst(ifconst) {}


    template <class RNG = PseudoRandom, class S = Statistics>
    class MakeMCDiscreteArithmeticAPConstEngine {
      public:
        MakeMCDiscreteArithmeticAPConstEngine(
            const boost::shared_ptr<GeneralizedBlackScholesProcess>& process, bool ifconst);
        // named parameters
        MakeMCDiscreteArithmeticAPConstEngine& withBrownianBridge(bool b = true);
        MakeMCDiscreteArithmeticAPConstEngine& withSamples(Size samples);
        MakeMCDiscreteArithmeticAPConstEngine& withAbsoluteTolerance(Real tolerance);
        MakeMCDiscreteArithmeticAPConstEngine& withMaxSamples(Size samples);
        MakeMCDiscreteArithmeticAPConstEngine& withSeed(BigNatural seed);
        MakeMCDiscreteArithmeticAPConstEngine& withAntitheticVariate(bool b = true);
        MakeMCDiscreteArithmeticAPConstEngine& withControlVariate(bool b = true);
        // conversion to pricing engine
        operator boost::shared_ptr<PricingEngine>() const;
      private:
        boost::shared_ptr<GeneralizedBlackScholesProcess> process_;
        bool antithetic_, controlVariate_;
        Size samples_, maxSamples_;
        Real tolerance_;
        bool brownianBridge_;
        BigNatural seed_;
        bool ifConst;
    };

    template <class RNG, class S>
    inline
    MakeMCDiscreteArithmeticAPConstEngine<RNG,S>::MakeMCDiscreteArithmeticAPConstEngine(
             const boost::shared_ptr<GeneralizedBlackScholesProcess>& process, bool ifconst)
    : process_(process), antithetic_(false), controlVariate_(false),
      samples_(Null<Size>()), maxSamples_(Null<Size>()),
      tolerance_(Null<Real>()), brownianBridge_(true), seed_(0), ifConst(ifconst) {}

    template <class RNG, class S>
    inline MakeMCDiscreteArithmeticAPConstEngine<RNG,S>&
    MakeMCDiscreteArithmeticAPConstEngine<RNG,S>::withSamples(Size samples) {
        QL_REQUIRE(tolerance_ == Null<Real>(),
                   "tolerance already set");
        samples_ = samples;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCDiscreteArithmeticAPConstEngine<RNG,S>&
    MakeMCDiscreteArithmeticAPConstEngine<RNG,S>::withAbsoluteTolerance(
                                                             Real tolerance) {
        QL_REQUIRE(samples_ == Null<Size>(),
                   "number of samples already set");
        QL_REQUIRE(RNG::allowsErrorEstimate,
                   "chosen random generator policy "
                   "does not allow an error estimate");
        tolerance_ = tolerance;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCDiscreteArithmeticAPConstEngine<RNG,S>&
    MakeMCDiscreteArithmeticAPConstEngine<RNG,S>::withMaxSamples(Size samples) {
        maxSamples_ = samples;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCDiscreteArithmeticAPConstEngine<RNG,S>&
    MakeMCDiscreteArithmeticAPConstEngine<RNG,S>::withSeed(BigNatural seed) {
        seed_ = seed;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCDiscreteArithmeticAPConstEngine<RNG,S>&
    MakeMCDiscreteArithmeticAPConstEngine<RNG,S>::withBrownianBridge(bool b) {
        brownianBridge_ = b;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCDiscreteArithmeticAPConstEngine<RNG,S>&
    MakeMCDiscreteArithmeticAPConstEngine<RNG,S>::withAntitheticVariate(bool b) {
        antithetic_ = b;
        return *this;
    }

    template <class RNG, class S>
    inline MakeMCDiscreteArithmeticAPConstEngine<RNG,S>&
    MakeMCDiscreteArithmeticAPConstEngine<RNG,S>::withControlVariate(bool b) {
        controlVariate_ = b;
        return *this;
    }

    template <class RNG, class S>
    inline
    MakeMCDiscreteArithmeticAPConstEngine<RNG,S>::operator boost::shared_ptr<PricingEngine>()
                                                                      const {
        return boost::shared_ptr<PricingEngine>(new
            MCDiscreteArithmeticAPConstEngine<RNG,S>(process_,
                                                brownianBridge_,
                                                antithetic_, controlVariate_,
                                                samples_, tolerance_,
                                                maxSamples_,
                                                seed_,
                                                ifConst));
    }



}


#endif
