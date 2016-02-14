CXXFLAGS=-Wall

all : equityoptiontest asianoptiontest 

equityoptiontest : ../src/blackscholesconstprocess.cpp equityoptiontest.cpp ../src/mceuropeanconstengine.hpp 
	g++ -g -o equityoptiontest ../src/blackscholesconstprocess.cpp equityoptiontest.cpp -l QuantLib

asianoptiontest : ../src/blackscholesconstprocess.cpp asianoptiontest.cpp ../src/mc_discr_arith_av_price_const.hpp 
	g++ -g -o asianoptiontest ../src/blackscholesconstprocess.cpp asianoptiontest.cpp -l QuantLib
