#include "data_handler.hpp"

data_handler::data_handler()
{
    data_array = new std::vector<data *>;
    training_data = new std::vector<data *>;
    test_data = new std::vector<data *>;
    validation_data = new std::vector<data *>;
}

data_handler::~data_handler()
{
    // TODO FREE dynamically allocated stuff
}

void data_handler::read_feature_vector(std::string path)
{
    uint32_t header[4]; // MAGIC | NUM IMAGES | ROWSIZE | COLSIZE
    unsigned char bytes[4];
    FILE *f = fopen(path.c_str(), "rb");
    if (f)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (fread(bytes, sizeof(bytes), 1, f))
            {
                header[i] = convert_to_little_endian(bytes);
            }
        }
        printf("Done Getting Data File Header.\n");
        int image_size = header[2] * header[3];
        for (int i = 0; i < header[1]; ++i)
        {
            data *d = new data();
            uint8_t element[1];
            for (int j = 0; j < image_size; ++j)
            {
                if (fread(element, sizeof(element), 1, f))
                {
                    d->append_to_feature_vector(element[0]);
                }
                else
                {
                    printf("Error While Reading File %s. \n", path);
                    exit(1);
                }
            }
            data_array->push_back(d);
        }
        printf("Successfully read and store %lu feature vectors.\n", data_array->size());
    }
    else
    {
        printf("Could Not Find File %s.\n", path);
        exit(1);
    }
}

void data_handler::read_feature_labels(std::string path)
{
    uint32_t header[2]; // MAGIC | NUM IMAGES
    unsigned char bytes[4];
    FILE *f = fopen(path.c_str(), "rb");
    if (f)
    {
        for (int i = 0; i < 2; ++i)
        {
            if (fread(bytes, sizeof(bytes), 1, f))
            {
                header[i] = convert_to_little_endian(bytes);
            }
        }
        printf("Done Getting Label File Header.\n");

        for (int i = 0; i < header[1]; ++i)
        {
            uint8_t element[1];

            if (fread(element, sizeof(element), 1, f))
            {
                data_array->at(i)->set_label(element[0]);
            }
            else
            {
                printf("Error while reading file %s. \n", path);
                exit(1);
            }
        }
        printf("Successfully read and store labels.\n");
    }
    else
    {
        printf("Could not find file %s.\n", path);
        exit(1);
    }
}

void data_handler::split_data()
{
    std::unordered_set<int> used_indexes;
    int train_size = data_array->size() * TRAIN_SET_PERCENT;
    int test_size = data_array->size() * TEST_SET_PERCENT;
    int validation_size = data_array->size() * VALIDATION_PERCENT;
    printf("Splitting data:\n");
    printf("\t-train_size: %u\n", train_size);
    printf("\t-test_size: %u\n", test_size);
    printf("\t-validation_size: %u\n", validation_size);
    printf("----------------------\n");

    srand(time(NULL));

    // // Training split
    // int count = 0;
    // while (count < train_size)
    // {
    //     int rand_index = rand() % data_array->size();
    //     if (used_indexes.find(rand_index) == used_indexes.end())
    //     {
    //         training_data->push_back(data_array->at(rand_index));
    //         printf("index %u\n", rand_index);
    //         used_indexes.insert(rand_index);
    //         count++;
    //         if (count % 1000 == 0)
    //         {
    //             printf("\tInserted a new data to training data %d\n", count);
    //         }
    //     }
    // }

    // // Test split
    // count = 0;
    // while (count < test_size)
    // {
    //     int rand_index = rand() % data_array->size();
    //     if (used_indexes.find(rand_index) == used_indexes.end())
    //     {
    //         test_data->push_back(data_array->at(rand_index));
    //         printf("index %u\n", rand_index);
    //         used_indexes.insert(rand_index);
    //         count++;
    //         if (count % 1000 == 0)
    //         {
    //             printf("\tInserted a new data to test data %d\n", count);
    //         }
    //     }
    // }

    // // Validation split
    // count = 0;
    // while (count < validation_size)
    // {
    //     int rand_index = rand() % data_array->size();
    //     printf("index %u\n", rand_index);
    //     if (used_indexes.find(rand_index) == used_indexes.end())
    //     {
    //         validation_data->push_back(data_array->at(rand_index));
    //         used_indexes.insert(rand_index);
    //         count++;
    //         if (count % 1000 == 0)
    //         {
    //             printf("\tInserted a new data to validation data %d\n", count);
    //         }
    //     }
    // }

    /**
     * ! The more optimizer way
     * ! generate a array of index from 0 to data_array->size()
     * ! then shuffle this array of index
     * ! get parts of this array respect to ratio of each corresponding size of training, test, and validation data
     */
    std::vector<int> *indexes = new std::vector<int>;

    for (int i = 0; i < data_array->size(); ++i)
    {
        indexes->push_back(i);
    }

    random_shuffle_indexes(indexes);

    for (int i = 0; i < data_array->size(); ++i)
    {
        if (i / train_size == 0)
        {
            training_data->push_back(data_array->at(indexes->at(i)));
        }
        else if (i / (train_size + test_size) == 0)
        {
            test_data->push_back(data_array->at(indexes->at(i)));
        }
        else
        {
            validation_data->push_back(data_array->at(indexes->at(i)));
        }
    }

    delete indexes;

    printf("Training Data Size: %lu (approx: %.2f %%)\n", training_data->size(), TRAIN_SET_PERCENT * 100.0);
    printf("Test Data Size: %lu (approx: %.2f %%)\n", test_data->size(), TEST_SET_PERCENT * 100.0);
    printf("Validation Data Size: %lu (approx: %.2f %%)\n", validation_data->size(), VALIDATION_PERCENT * 100.0);
}

void data_handler::count_classes()
{
    int count = 0;
    for (unsigned i = 0; i < data_array->size(); ++i)
    {
        uint8_t label = data_array->at(i)->get_label();
        if (class_map.find(label) == class_map.end())
        {
            class_map[label] = (int)label;
            data_array->at(i)->set_enumerated_label(count);
            count++;
        }
    }
    num_classes = count;
    printf("Successfully Extracted %d Unique Classes.\n", num_classes);
}

uint32_t data_handler::convert_to_little_endian(const unsigned char *bytes)
{
    return (uint32_t)((bytes[0] << 24) |
                      (bytes[1] << 16) |
                      (bytes[2] << 8) |
                      (bytes[3]));
}

std::vector<data *> *data_handler::get_training_data()
{
    return training_data;
}

std::vector<data *> *data_handler::get_test_data()
{
    return test_data;
}

std::vector<data *> *data_handler::get_validation_data()
{
    return validation_data;
}

void data_handler::random_shuffle_indexes(std::vector<int> *indexes)
{
    srand(time(NULL));
    unsigned int size = indexes->size();
    for (int _ = 0; _ < size; ++_)
    {
        int random_index1 = rand() % size;
        int random_index2 = rand() % size;
        indexes->at(random_index1) = indexes->at(random_index1) + indexes->at(random_index2);
        indexes->at(random_index2) = indexes->at(random_index1) - indexes->at(random_index2);
        indexes->at(random_index1) = indexes->at(random_index1) - indexes->at(random_index2);
    }
}
