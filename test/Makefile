CXXFLAGS=-Wall

all : equityoptiontest blackscholesconsttest

equityoptiontest : equityoptiontest.cpp
	g++ -g -o equityoptiontest equityoptiontest.cpp -l QuantLib

blackscholesconsttest : ../src/blackscholesconstprocess.cpp blackscholesconsttest.cpp 
	g++ -g -o blackscholesconsttest ../src/blackscholesconstprocess.cpp blackscholesconsttest.cpp -l QuantLib
