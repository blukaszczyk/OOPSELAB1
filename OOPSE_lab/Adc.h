#pragma once
#include <string>
#include <chrono>
#include <vector>
#include <sstream>
#include <cmath>

template <typename T=short, int N=1024>
struct ADC {
    int bits;
    std::chrono::milliseconds clock_rate;
    double rangeMin, rangeMax;
    double sensitivity;
    double trigger_level;
    std::vector<T> samples;

public:
    T convert(double voltage) {
        if (voltage < rangeMin) voltage = rangeMin;
        if (voltage > rangeMax) voltage = rangeMax;

        long long max_digital_value = (1LL << bits) - 1;

        double proportion = (voltage - rangeMin) / (rangeMax - rangeMin);
        return static_cast<T>(std::round(proportion * max_digital_value));
    }

    size_t read_input(std::vector<double> inputs) {
        size_t initial_size = samples.size();
        for (double input : inputs) {
            if (input >= trigger_level) {
                samples.push_back(convert(input));
            }
        }
        return samples.size() - initial_size;
    }

    double value(size_t i) {
        if (i >= samples.size()) return 0.0;
        
        long long max_digital_value = (1LL << bits) - 1;

        double voltage = (static_cast<double>(samples[i]) / max_digital_value) * (rangeMax - rangeMin) + rangeMin;

        return voltage * sensitivity;
    }

    std::vector<double> values() {
        std::vector<double> true_values;
        true_values.reserve(samples.size());
        for (size_t i = 0; i < samples.size(); ++i) {
            true_values.push_back(value(i));
        }
        return true_values;
    }

    std::string to_string() {
        std::ostringstream oss;
        oss << "ADC [" << bits << "-bit | " 
            << "Range: " << rangeMin << "V to " << rangeMax << "V | "
            << "Samples: " << samples.size() << "]";
        return oss.str();
    }
};