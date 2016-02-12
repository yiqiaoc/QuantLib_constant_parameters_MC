# Prototype of first version 
##### subject 1 - use constant parameters in ```Monte Carlo engines```
## I Context
Monte Carlo engine provides a methodology to calculate the value of an option with multiple sources of uncertainty or given features [Wikipedia]. In the ```Quantlib``` library, this is related to the ```Drift``` and ```Diffusion``` method, the multiple calls of which may cause a decreased performance, especially when it is represented by a ```GeneralizedBlackScholesProcess``` class instance. 

At the same time, passed parameters accuracy has an impact on the performance as well. We propose thus to change these parameters to constant values in order to study this influence with a new-created Black-Scholes process class and a modified ```MCEuropeanEngine``` class who takes an original Black-Sholes process as parameter. This class accepts also a boolean value, allowing to decide if the constant parameters will be used.

## II Solution
##### 1.Modifications for constants
###### 1.1 Four values are set to be constant
The performance of the engine can be increased at the expense of some accuracy. We want to create a new class that models a Black-Scholes process with constant parameters. And we decide to determinate the four parameters underlying value, risk-free rate, dividend yield, and volatility to be constant.
###### 1.2 How to determinate the constant values
```Underlying value``` : take the underlying value at t=0 (S0)

```Risk-free rate``` : take the zero-rate at T (maturity) from the risk-free interest-rate curve

```Dividend yield```: take the zero-rate at T from the dividend-yield curve

```Volatility``` : take it from the blackVol method at T and feed it directly to the process

##### 2.Creation of new Black-Scholes process class
The new Black-Scholes process class inherits from ```StochasticProcess1D``` class and takes several constant parameters which are calculated in the constructor. In order to reduce calculations, we modified ```diffusion()```, ```drift()```  and constructor of ```GeneralizedBlackScholesProcess```, then removed a few useless methods like ```localvolatility()``` and ```update()```. Those constant parameters only need to be calculated one time in the constructor by using our proposed methods. You can find four new parameters and their calculate functions below :
```c++
riskFreeForward_ = riskFreeRate_->zeroRate(T, Continuous, NoFrequency, true);
dividendForward_ = dividendYield_->zeroRate(T, Continuous, NoFrequency, true);
sigma = blackVolatility_->blackVol(T, x0->value(), true);
drift_ = riskFreeForward_ - dividendForward_ - 0.5 * sigma * sigma;
//*T: numbers of years.
```
##### 3.Modifications of Monte Carlo Engine
In order to avoid compiling the whole Quantlib project, we decided to inherit the class ```MCEuropeanEngine``` instead of modifying it. This new class names ```MCEuropeanConstEngine``` and overrides the method ```pathGenerator()```. 
In that method, we will construct a new ```BlackScholesConstProcess``` and apply this constant process to the same program. Thus, we need to extract the main values needed by the constructor of ```BlackScholesConstProcess``` from the input process :
```c++
boost::shared_ptr<path_generator_type> pathGenerator() const {
    boost::shared_ptr<BlackScholesConstProcess> constProcess_(
        new BlackScholesConstProcess(
            realProcess->stateVariable(),
            realProcess->dividendYield(),
            realProcess->riskFreeRate(),
            realProcess->blackVolatility()                                              
    ));
        
    Size dimensions = constProcess_->factors();
    TimeGrid grid = this->timeGrid();
    typename RNG::rsg_type generator =                     RNG::make_sequence_generator(dimensions*(grid.size()-1),seed_);
                    cout<<"this is a const result"<<endl;
    return boost::shared_ptr<path_generator_type>(
            new path_generator_type(constProcess_, grid,
                            generator, brownianBridge_));
                    
};
```
## III Next steps
##### 1. Steps to carry based on obtained results
For instance, the realised test still has some drawbacks which limit the verification of the new-created engine result and performance . Thus, in the next step we’ll try to find a more adapted scenario to test the new engine efficiency. 
In the same time, we will also work on the same modification to another class -- ```MCDiscreteArithmeticAPEngine``` class. 
##### 2. Difficulties 
###### 2.1 MCEuropeanEngine
The ```setPricingEngine``` method of ```VanillaOption``` class takes a PricingEngine type object. Even we modified ```MCEuropeanEngine``` class by adding a new boolean parameter, it wouldn’t be used in the process because it took its parent class type.  So in our current solution, we created a new class named ```MCEuropeanConstEngine``` which uses directly new Black-Scholes constant process in its ```pathGenerator``` method. 
###### 2.2 Maturity
The maturity is set in the ```EuropeanOption``` class. We didn’t know how to pass settlement day and maturity informations (i.e time parameter) to new Black-Scholes const process so as to calculate ```zerorate``` and ```blackVol```. 


