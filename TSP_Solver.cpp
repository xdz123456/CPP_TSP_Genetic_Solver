//20031862 scydx2 Dongzhan Xie
//If the number of city is less than 1000, the program should run 10min
//If the number of city is bigger than 1000, the program should run 20min
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctype.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>
#include <float.h>

using namespace std;

//init the city class
class city {
public:
    void init(int city_name, double city_x, double city_y);

    int get_name();

    double get_x();

    double get_y();

private:
    int city_name;
    double city_x;
    double city_y;
};

void city::init(int city_name, double city_x, double city_y) {
    this->city_name = city_name;
    this->city_x = city_x;
    this->city_y = city_y;
}

int city::get_name() {
    return this->city_name;
}

double city::get_x() {
    return this->city_x;
}

double city::get_y() {
    return this->city_y;
}

class solution {
public:
    void set_sol(vector<int> my_solution);

    vector<int> get_sol();

    void Genetic_Algorithm(vector<city> city_list);

private:
    vector<int> my_solution;

    double cal_dis(vector<city> city_list, vector<int> solution);

    double cal_dis_each(vector<city> city_list, int city1, int city2);

    vector<int> greedy(vector<city> city_list);

    void init_population(vector<city> city_list, vector<int> *pop, int SIZE_OF_POP);

    vector<int> select(vector<city> city_list, vector<int> *pop, int SIZE_OF_POP);

    vector<int> mate(vector<city> city_list, vector<int> *pop, int SIZE_OF_POP);

    vector<int> local_search(vector<city> city_list, vector<int> child);

    void print(vector<city> city_list);
};

void solution::set_sol(vector<int> my_solution) {
    this->my_solution = my_solution;
}

vector<int> solution::get_sol() {
    return this->my_solution;
}

//calculate the total distance of one solution
double solution::cal_dis(vector<city> city_list, vector<int> solution) {
    double distance = 0;
    int length = solution.size();
    for (int i = 0; i < length - 1; i++) {
        double x1 = city_list[solution[i]].get_x();
        double y1 = city_list[solution[i]].get_y();
        double x2 = city_list[solution[i + 1]].get_x();
        double y2 = city_list[solution[i + 1]].get_y();
        distance += sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    }
    return distance;
}

//calculate the distance between the city
double solution::cal_dis_each(vector<city> city_list, int city1, int city2) {
    double x1 = city_list[city1].get_x();
    double y1 = city_list[city1].get_y();
    double x2 = city_list[city2].get_x();
    double y2 = city_list[city2].get_y();
    double distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    return distance;
}

//run the greedy algorithm to find the solution
vector<int> solution::greedy(vector<city> city_list) {
    int length = city_list.size();
    //init the flag
    vector<int> flag;
    for (int i = 0; i < length - 1; i++) {
        flag.push_back(0);
    }
    vector<int> solution;
    solution.push_back(0);
    //find the minimum distance in each city, if the city didn't reach
    for (int i = 0; i < length - 1; i++) {
        double temp_dis = DBL_MAX;
        int temp_index = 0;
        for (int j = 1; j < length; j++) {
            if (temp_dis > cal_dis_each(city_list, solution[i], j) && flag[j - 1] == 0) {
                temp_dis = cal_dis_each(city_list, solution[i], j);
                temp_index = j;
            }
        }
        flag[temp_index - 1] = 1;
        solution.push_back(temp_index);
    }
    //return the solution
    solution.push_back(0);
    return solution;
}

//Init the population by using greedy algorithm
void solution::init_population(vector<city> city_list, vector<int> *pop, int SIZE_OF_POP) {
    vector<int> solution = greedy(city_list);
    pop[0] = solution;
    int length = solution.size();
    for (int k = 1; k < SIZE_OF_POP; k++) {
        vector<int> temp = solution;
        for (int i = 0; i < length * 0.1; i++) {
            int rand1 = rand() % (length - 2) + 1;
            int rand2 = rand() % (length - 2) + 1;
            int temp_num = temp[rand1];
            temp[rand1] = temp[rand2];
            temp[rand2] = temp_num;
        }
        pop[k] = temp;
    }
}

//Using the roulette to select the parent
vector<int> solution::select(vector<city> city_list, vector<int> *pop, int SIZE_OF_POP) {
    //calculate the the sum of the reciprocal distances
    double total_dis = 0;
    for (int i = 0; i < SIZE_OF_POP; i++) {
        total_dis += 1 / cal_dis(city_list, pop[i]);
    }
    //store the probability into the wheel
    vector<double> wheel;
    double probability;
    for (int i = 0; i < SIZE_OF_POP; i++) {
        probability += 1 / cal_dis(city_list, pop[i]) / total_dis;
        wheel.push_back(probability);
    }
    double rand_num = 1.0 * rand() / RAND_MAX;
    int index = 0;
    //Using the roulette to select the parent
    for (int i = 0; i < SIZE_OF_POP; i++) {
        if (rand_num < wheel[i]) {
            index = i;
            break;
        }
    }
    //return the parent
    vector<int> parent = pop[index];
    return parent;
}


vector<int> solution::local_search(vector<city> city_list, vector<int> child) {
    int length = city_list.size();
    double distance = cal_dis(city_list, child);

    for (int j = 0; j < length; j++) {
        int random = rand() % (length - 1) + 1;
        for (int i = 1; i < length; i++) {
            int temp_1 = child[random];
            child[random] = child[i];
            child[i] = temp_1;
            if (distance > cal_dis(city_list, child)) {

                return child;
            } else {
                int temp_2 = child[random];
                child[random] = child[i];
                child[i] = temp_2;
            }
        }
    }
    return child;
}

//Obtain the child solution by mating 29499
vector<int> solution::mate(vector<city> city_list, vector<int> *pop, int SIZE_OF_POP) {
    //Init the parent solution
    vector<int> parent_1 = select(city_list, pop, SIZE_OF_POP);
    vector<int> child = local_search(city_list, parent_1);
    return child;
}

//Run the Genetic Algorithm to get the better solution
void solution::Genetic_Algorithm(vector<city> city_list) {
    //Init population's size
    int SIZE_OF_POP = 10;
    //Init population which include solutions
    vector<int> pop[SIZE_OF_POP];
    //Init the max time
    int length = city_list.size();
    double MAX_TIME;
    if (length < 1000) {
        MAX_TIME = 580;
    } else {
        MAX_TIME = 1180;
    }
    clock_t time_start = clock();
    //Use init_population function to init the population
    init_population(city_list, pop, SIZE_OF_POP);
    //Selete parent with Roulette
    //Mate 8000 times and rplace the bad one in populatuon
    for (int i = 0; i < 5000; ++i) {
        //cout << i <<"\n";
        vector<int> child = mate(city_list, pop, SIZE_OF_POP);
        //Replace the worst one in the population
        int bad_index = 0;
        double temp_0 = 0;
        for (int i = 0; i < SIZE_OF_POP; i++) {
            if (temp_0 <= cal_dis(city_list, pop[i])) {
                temp_0 = cal_dis(city_list, pop[i]);
                bad_index = i;
            }
        }
        pop[bad_index].clear();
        pop[bad_index] = child;
        //If time spend is larger than MAX_TIME, break
        clock_t time_end = clock();
        double time_spend = (time_end - time_start) / (double) CLOCKS_PER_SEC;
        if (time_spend > MAX_TIME) { break; }
    }
    //Find the best solution in the whole population
    int good_index;
    double temp = DBL_MAX;
    for (int i = 0; i < SIZE_OF_POP; i++) {
        if (cal_dis(city_list, pop[i]) < temp) {
            temp = cal_dis(city_list, pop[i]);
            good_index = i;
        }
    }
    this->my_solution = pop[good_index];
    print(city_list);
}

void solution::print(vector<city> city_list) {
    cout << cal_dis(city_list, this->my_solution) << "\t";
    int length = this->my_solution.size();
    for (int i = 0; i < length; ++i) {
        cout << city_list[this->my_solution[i]].get_name() << "\t";
    }
    cout << "\n";
}

//Main function
int main(int argc, char const *argv[]) {
    //Check the input
    if (argc != 2) {
        cout << "Wrong arguments, the argument should include the question file.\n";
        exit(0);
    }
    //Obtain the filename
    string file_name = argv[1];
    //If enter the wrong file name, exit
    ifstream ist{file_name};
    if (!ist) {
        cout << "Can not open the file " << file_name << "\n";
        exit(0);
    }
    //Init the city's value
    int city_name;
    double city_x;
    double city_y;
    //init
    vector<city> city_list;
    vector<int> answer;
    city temp;
    //Skip the first line
    string firt_line;
    getline(ist, firt_line);
    //Use the stream to store the city's information into the city_list
    while (ist && ist.good()) {
        ist >> city_name >> city_x >> city_y;
        temp.init(city_name, city_x, city_y);
        city_list.push_back(temp);
    }
    //Obtain the random seed by time
    srand(time(NULL));
    //Run the GA, and obtain the best solution
    solution sol;
    sol.Genetic_Algorithm(city_list);
    return 0;
}
