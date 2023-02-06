### Here is a TSP Solver implement by the Genetic algorithm

To run this program use flowing code in terminal:

```
g++ -std=c++11 TSP_Solver.cpp -o tsp
./tsp example1.txt
```

where txt files are the example TSP problem

The Solver will run 10 minutes running time for each problem instance with less than 1000 cities and 20 minutes for instances with 1000+ cities.