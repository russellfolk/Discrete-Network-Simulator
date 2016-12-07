#! /usr/bin/make
# Makefile for Discrete Network Simulator

all: simulator create_graph

simulator:
	g++ -o bin/simulator -pipe -O2 -std=c++11 -ggdb -Wall -Iinclude src/simulator.cpp src/router.cpp src/incoming_connection.cpp src/outgoing_connection.cpp src/uniform_random_variable.cpp src/poisson_random_variable.cpp

create_graph:
	g++ -o bin/create_graph -pipe -O2 -std=c++11 -ggdb -Wall src/CreateNetworkGraph.cpp