#include "knn.hpp"

knn::knn(int val)
{
    k = val;
}

knn::knn()
{
    training_data = new std::vector<data *>;
    test_data = new std::vector<data *>;
    validation_data = new std::vector<data *>;
}

knn::~knn()
{
    delete training_data;
    delete test_data;
    delete validation_data;
}

void knn::find_knearest(data *query_point)
{
    neighbors = new std::vector<data *>;
    double min = std::numeric_limits<double>::max();
    double previous_min = min;
    int index = 0;
    for (int i = 0; i < k; i++)
    {
        if (i = 0)
        {
            for (int j = 0; j < training_data->size(); ++j)
            {
                training_data->at(j)->set_distance(calculate_distance(query_point, training_data->at(j)));

                double distance = training_data->at(j)->get_distance();

                if (distance < min)
                {
                    min = distance;
                    index = j;
                }
            }
            // neighbors->push_back(training_data->at(index));
            // previous_min = min;
            // min = std::numeric_limits<double>::max();
        }
        else
        {
            for (int j = 0; j < training_data->size(); j++)
            {
                double distance = training_data->at(j)->get_distance();
                if (distance < min && distance > previous_min)
                {
                    min = distance;
                    index = j;
                }
            }
        }
        neighbors->push_back(training_data->at(index));
        previous_min = min;
        min = std::numeric_limits<double>::max();
    }
}

void knn::set_training_data(std::vector<data *> *input_data)
{
    training_data = input_data;
}

void knn::set_test_data(std::vector<data *> *input_data)
{
    test_data = input_data;
}

void knn::set_validation_data(std::vector<data *> *input_data)
{
    validation_data = input_data;
}

void knn::set_k(int val)
{
    k = val;
}

uint8_t knn::predict()
{
    std::map<uint8_t, int> class_freq;
    for (int i = 0; i < neighbors->size(); ++i)
    {
        uint8_t label = neighbors->at(i)->get_label();
        std::pair<std::map<uint8_t, int>::iterator, bool> emplace_pair = class_freq.emplace(label, 0);
        emplace_pair.first->second += 1;
    }

    int best = 0;
    int max = 0;
    for (auto kv : class_freq)
    {
        if (kv.second > max)
        {
            max = kv.second;
            best = kv.first;
        }
    }

    delete neighbors;
    return best;
}

double knn::calculate_distance(data *query_point, data *input)
{
    // https://math.stackexchange.com/questions/139600/how-do-i-calculate-euclidean-and-manhattan-distance-by-hand

    long double distance = 0.0;
    std::vector<uint8_t> *query_feature_vector = query_point->get_feature_vector();
    std::vector<uint8_t> *input_feature_vector = input->get_feature_vector();
    if (query_feature_vector->size() != input_feature_vector->size())
    {
        printf("Error vector size mismatch.\n");
        exit(1);
    }
#ifdef EUCLID
    // TODO Optimize math operation and loop

    for (unsigned i = 0;
         i < query_feature_vector->size();
         ++i)
    {
        distance += exp((query_feature_vector->at(i) - input_feature_vector->at(i)) * log(2));
    }
    distance = sqrt(distance);

#elif defined MANHATTAN
    // TODO calculate manhattan distance

    for (unsigned i = 0;
         i < query_feature_vector->size(); // TODO: optimization
         i++)
    {
        distance += abs(query_feature_vector->at(i) - input_feature_vector->at(i));
    }
#endif
    return distance;
}

double knn::validate_performance()
{
    printf("Validation dataset now has %d samples", validation_data->size());
    double result_performance = 0;
    int correct_predict = 0;
    int count_predicted = 0;
    for (data *query_point : *validation_data)
    {
        find_knearest(query_point); // This function will update this.neighbors
        uint8_t predict_label = predict();
        count_predicted++;

        // compare and count thhe correct predicted
        correct_predict += (predict_label == query_point->get_label()) ? 1 : 0;
        printf("The current perfomance on validation dataset for K = %d is %.3f %%\n", k, ((double)correct_predict * 100.0) / (double)count_predicted);
    }

    result_performance = ((double)correct_predict * 100.0) / (double)validation_data->size();
    printf("\t\t===> Validation performance for K = %d is %.3f %%\n", k, result_performance);

    return result_performance;
}

double knn::test_performance()
{
    printf("Test dataset now has %d samples", validation_data->size());
    double result_performance = 0;
    int correct_predict = 0;
    int count_predicted = 0;
    for (data *query_point : *test_data)
    {
        find_knearest(query_point); // This function will update this.neighbors
        uint8_t predict_label = predict();
        count_predicted++;

        // compare and count the correct predicted
        correct_predict += (predict_label == query_point->get_label()) ? 1 : 0;
        printf("The current perfomance on test dataset K = %d is %.3f %%\n", k, ((double)correct_predict * 100.0) / (double)count_predicted);
    }

    result_performance = ((double)correct_predict * 100.0) / (double)validation_data->size();
    printf("\t\t===> Test performance for K = %d is %.3f %%\n", k, result_performance);

    return result_performance;
}
