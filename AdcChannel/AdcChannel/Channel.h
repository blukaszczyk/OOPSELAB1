// Channel.h
#pragma once
#include <string>
#include <vector>
#include "Adc.h"

class Channel {
private:
    ADC<short, 1024>& adc;
    int channel_index;
    bool is_running;

public:
    Channel(ADC<short, 1024>& adc_ref, int index);
    Channel(const Channel& other);
    void run();
    std::vector<double> get_values() const;
    std::string to_string() const;
};