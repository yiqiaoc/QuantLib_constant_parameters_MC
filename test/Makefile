CXXFLAGS=-Wall

all : equityoptiontest 

equityoptiontest : ../src/blackscholesconstprocess.cpp equityoptiontest.cpp ../src/mceuropeanconstengine.hpp 
	g++ -g -o equityoptiontest ../src/blackscholesconstprocess.cpp equityoptiontest.cpp -l QuantLib
