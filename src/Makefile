CXXFLAGS=-Wall

all : BlackScholesConstProcess MCEuropeanConstEngine

BlackScholesConstProcess : BlackScholesConstProcess.hpp
	g++ -g -o BlackScholesConstProcess BlackScholesConstProcess.hpp -l QuantLib

MCEuropeanConstEngine : MCEuropeanConstEngine.hpp
	g++ -g -o MCEuropeanConstEngine MCEuropeanConstEngine.hpp -l QuantLib
