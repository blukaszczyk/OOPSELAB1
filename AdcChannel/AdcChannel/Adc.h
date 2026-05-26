// Adc.h
#pragma once
#include <string>
#include <chrono>
#include <vector>

template <typename T = short, int N = 1024>
struct ADC {
    int bits;
    std::chrono::milliseconds clock_rate;
    double rangeMin, rangeMax;
    double sensitivity;
    double trigger_level;
    std::vector<T> samples;

    T convert(double voltage) {
        if (voltage <= rangeMin) return 0;
        if (voltage >= rangeMax) return (1 << bits) - 1;
        double normalized = (voltage - rangeMin) / (rangeMax - rangeMin);
        return static_cast<T>(normalized * ((1 << bits) - 1));
    }

    size_t read_input(std::vector<double>& inputs) {
        size_t count = 0;
        for (const auto& voltage : inputs) {
            T sample = convert(voltage);
            if (voltage >= trigger_level) {
                samples.push_back(sample);
                count++;
            }
        }
        return count;
    }

    double value(size_t i) {
        if (i >= samples.size()) return 0.0;
        double normalized = samples[i] / static_cast<double>((1 << bits) - 1);
        double voltage = rangeMin + normalized * (rangeMax - rangeMin);
        return voltage * sensitivity;
    }

    std::vector<double> values() {
        std::vector<double> result;
        for (size_t i = 0; i < samples.size(); i++) {
            result.push_back(value(i));
        }
        return result;
    }

    std::string to_string() {
        std::string result = "ADC samples:\n";
        for (size_t i = 0; i < samples.size(); i++) {
            result += "Sample " + std::to_string(i) + ": " + std::to_string(value(i)) + "\n";
        }
        return result;
    }
};