#include "data.hpp"
#include "knn.hpp"
#include "data_handler.hpp"

int main()
{
    data_handler *dh = new data_handler();
    dh->read_feature_vector("data/train-images-idx3-ubyte");
    dh->read_feature_labels("data/train-labels-idx1-ubyte");

    dh->split_data();
    dh->count_classes();

    knn *knearest = new knn();
    knearest->set_training_data(dh->get_training_data());
    knearest->set_test_data(dh->get_test_data());
    knearest->set_validation_data(dh->get_validation_data());
    double perfomance = 0.0;
    double best_performance = 0;
    int best_k = 1;
    for (int i = 1; i <= 4; ++i)
    {
        knearest->set_k(i);
        perfomance = knearest->validate_performance();
        if(i == 1) {
            best_performance = perfomance;    
        } else {
            if (perfomance < best_performance) {
                best_performance = perfomance;
                best_k = i;
            }
        }
    }
    printf("Best Perfomance: %.3d", best_performance);
    printf("Best K: %d", best_k);
}