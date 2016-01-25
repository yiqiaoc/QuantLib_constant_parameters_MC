#include <ql/quantlib.hpp>
#include <boost/timer.hpp>
#include <iomanip>
#include "../src/mceuropeanconstengine.hpp"

using namespace QuantLib;

int main(int argc, char* argv[]){
    
    try{
        
        boost::timer timer;
        std::cout << std::endl;

        // set up dates
        Calendar calendar = TARGET();
        Date todaysDate(15, May, 1998);
        Date settlementDate(17, May, 1998);
        Settings::instance().evaluationDate() = todaysDate;

        // our option parameters
        Option::Type type(Option::Put);
        Real underlying = 36;
        Real strike = 40;
        Spread dividendYield = 0.00;
        Rate riskFreeRate = 0.06;
        Volatility volatility = 0.20;
        Date maturity(17, May, 1999);
        DayCounter dayCounter = Actual365Fixed();

        std::cout << "Option type = "  << type << std::endl;
        std::cout << "Maturity = "        << maturity << std::endl;
        std::cout << "Underlying price = "        << underlying << std::endl;
        std::cout << "Strike = "                  << strike << std::endl;
        std::cout << "Risk-free interest rate = " << io::rate(riskFreeRate)
                  << std::endl;
        std::cout << "Dividend yield = " << io::rate(dividendYield)
                  << std::endl;
        std::cout << "Volatility = " << io::volatility(volatility)
                  << std::endl;
        std::cout << std::endl;
        std::string method;
        std::cout << std::endl ;


        // underlying handler
        Handle<Quote> underlyingH(
                boost::shared_ptr<Quote>(new SimpleQuote(underlying)));

        // bootstrap the yield/dividend/vol curves
        Handle<YieldTermStructure> flatTermStructure(
            boost::shared_ptr<YieldTermStructure>(
                new FlatForward(settlementDate, riskFreeRate, dayCounter)));
        Handle<YieldTermStructure> flatDividendTS(
            boost::shared_ptr<YieldTermStructure>(
                new FlatForward(settlementDate, dividendYield, dayCounter)));
        Handle<BlackVolTermStructure> flatVolTS(
            boost::shared_ptr<BlackVolTermStructure>(
                new BlackConstantVol(settlementDate, calendar, volatility,
                                     dayCounter)));
 

        // european exercise
        boost::shared_ptr<Exercise> europeanExercise(
                new EuropeanExercise(maturity));

        // payoff
        boost::shared_ptr<StrikedTypePayoff> payoff(
                new PlainVanillaPayoff(type, strike));

        // BlackScholes Merton Process
        boost::shared_ptr<BlackScholesMertonProcess> bsmProcess(
                new BlackScholesMertonProcess(underlyingH, flatDividendTS, flatTermStructure, flatVolTS));

        // options
        VanillaOption europeanOption(payoff, europeanExercise);

        // Black-Scholes for European
        europeanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                    new AnalyticEuropeanEngine(bsmProcess)));
        std::cout << "Black-Scholes : " << europeanOption.NPV() << std::endl;

        // Monte Carlo Method: MC (crude)
        Size timeSteps = 1;
        Size mcSeed = 42;
        boost::shared_ptr<PricingEngine> mcengine1;
        mcengine1 = MakeMCEuropeanEngine<PseudoRandom>(bsmProcess)
            .withSteps(timeSteps)
            .withAbsoluteTolerance(0.02)
            .withSeed(mcSeed);
        europeanOption.setPricingEngine(mcengine1);
        // Real errorEstimate = europeanOption.errorEstimate();
        std::cout << "MC (crude) : " << europeanOption.NPV() << std::endl;
        
        boost::shared_ptr<PricingEngine> mcengine1c;
        mcengine1c = MakeMCEuropeanConstEngine<PseudoRandom>(bsmProcess)
            .withSteps(timeSteps)
            .withAbsoluteTolerance(0.02)
            .withSeed(mcSeed);
        europeanOption.setPricingEngine(mcengine1c);
        // Real errorEstimate = europeanOption.errorEstimate();
        std::cout << "MC const(crude) : " << europeanOption.NPV() << std::endl;

        // Monte Carlo Method: QMC (Sobol)
        Size nSamples = 32768;  // 2^15

        boost::shared_ptr<PricingEngine> mcengine2;
        mcengine2 = MakeMCEuropeanEngine<LowDiscrepancy>(bsmProcess)
            .withSteps(timeSteps)
            .withSamples(nSamples);
                 
        europeanOption.setPricingEngine(mcengine2);
        std::cout << "MC (Sobol) : " << europeanOption.NPV() << std::endl;
        
        boost::shared_ptr<PricingEngine> mcengine2c;
        mcengine2c = MakeMCEuropeanEngine<LowDiscrepancy>(bsmProcess)
            .withSteps(timeSteps)
            .withSamples(nSamples);
                 
        europeanOption.setPricingEngine(mcengine2c);
        std::cout << "MC const (Sobol) : " << europeanOption.NPV() << std::endl;

        // End test
        double seconds = timer.elapsed();
        Integer hours = int(seconds/3600);
        seconds -= hours * 3600;
        Integer minutes = int(seconds/60);
        seconds -= minutes * 60;
        std::cout << " \nRun completed in ";
        if (hours > 0)
            std::cout << hours << " h ";
        if (hours > 0 || minutes > 0)
            std::cout << minutes << " m ";
        std::cout << std::fixed << std::setprecision(0)
                  << seconds << " s\n" << std::endl;
        return 0;

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "unknown error" << std::endl;
        return 1;
    }

}

