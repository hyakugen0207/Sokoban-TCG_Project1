bds:
	g++ -std=c++11 -O2 src/shiroBoard2.cpp src/shiroHashMap3.cpp src/solverBDS2.cpp -c 
	g++ -std=c++11 -O2 shiroBoard2.o shiroHashMap3.o solverBDS2.o -o shiroBDS
	rm -f *.o