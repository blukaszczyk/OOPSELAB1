// main.cpp
#include <iostream>
#include "Adc.h"
#include "Channel.h"

int main() {
    ADC<short, 1024> adc;
    adc.bits = 10;
    adc.clock_rate = std::chrono::milliseconds(10);
    adc.rangeMin = 0.0;
    adc.rangeMax = 3.3;
    adc.sensitivity = 1.0;
    adc.trigger_level = 1.65;

    Channel channel(adc, 0);
    channel.run();

    std::vector<double> inputs = { 0.5, 1.0, 1.7, 2.2, 3.0 };
    adc.read_input(inputs);

    std::vector<double> values = channel.get_values();
    std::cout << "Channel values:" << std::endl;
    for (size_t i = 0; i < values.size(); i++) {
        std::cout << "  " << i << ": " << values[i] << std::endl;
    }

    std::cout << adc.to_string() << std::endl;
    std::cout << channel.to_string() << std::endl;

    return 0;
}