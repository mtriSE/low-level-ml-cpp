#ifndef __KNN_H
#define __KNN_H

#define MANHATTAN
// #define EUCLID

#include <vector>
#include <cmath>
#include <limits>
#include <map>
#include "stdint.h"
#include "data.hpp"
#include "data_handler.hpp"

class knn
{
private:
    int k;
    std::vector<data *> *neighbors;
    std::vector<data *> *training_data;
    std::vector<data *> *test_data;
    std::vector<data *> *validation_data;

public:
    knn(int);
    knn();
    ~knn();

    void find_knearest(data *);
    void set_training_data(std::vector<data *> *);
    void set_test_data(std::vector<data *> *);
    void set_validation_data(std::vector<data *> *);
    void set_k(int);

    uint8_t predict();
    double calculate_distance(data *, data *);
    double validate_performance();
    double test_performance();
};

#endif // __KNN_H
