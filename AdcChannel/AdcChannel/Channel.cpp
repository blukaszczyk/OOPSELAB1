// Channel.cpp
#include "Channel.h"
#include <sstream>

Channel::Channel(ADC<short, 1024>& adc_ref, int index)
    : adc(adc_ref), channel_index(index), is_running(false) {}

Channel::Channel(const Channel& other)
    : adc(other.adc), channel_index(other.channel_index), is_running(other.is_running) {}

void Channel::run() {
    is_running = true;
}

std::vector<double> Channel::get_values() const {
    std::vector<double> values;
    if (is_running) {
        values = adc.values();
    }
    return values;
}

std::string Channel::to_string() const {
    std::ostringstream oss;
    oss << "Channel " << channel_index << ": ";
    if (is_running) {
        oss << "Running, " << adc.samples.size() << " samples collected";
    }
    else {
        oss << "Not running";
    }
    return oss.str();
}