#include <gtest/gtest.h>
#include "Channel.h"

struct ChannelFixture : public ::testing::Test {
    std::vector<double> inputs {1., 2., 3., 4., 1.5, 1.99, 2.5, 3.5, 4.5, 4.99};
    int bits {16};
    std::chrono::milliseconds clock_rate {100};
    double rangeMin {-5.}, rangeMax {5.};
    double sensitivity {1.}, trigger_level {2.};
    ADC<unsigned short, 4> adc {bits, clock_rate, rangeMin, rangeMax, sensitivity, trigger_level};
    Channel<unsigned short, 4> channel {adc};
};

TEST_F(ChannelFixture, AdcConvertBoundaries) {
    auto adc = channel.get_adc();
    long long max_digital_val = (1LL << 16) - 1;

    EXPECT_EQ(0, adc.convert(-5.0));
    EXPECT_EQ(max_digital_val, adc.convert(5.0));

    EXPECT_EQ(32768, adc.convert(0.0));

    EXPECT_EQ(0, adc.convert(-10.0));

    EXPECT_EQ(max_digital_val, adc.convert(10.0));
}

TEST_F(ChannelFixture, ReadInputTriggerLogic) {
    size_t processed_count = channel.run(inputs);

    EXPECT_EQ(7, processed_count);

    EXPECT_EQ(7, channel.get_adc().samples.size());
}

TEST_F(ChannelFixture, ValueReverseConversion) {
    channel.run(inputs);
    auto values = channel.get_values();

    EXPECT_EQ(0.0, channel.get_adc().value(99));

    EXPECT_EQ(7, values.size());

    EXPECT_NEAR(2.0, values[0], 0.0002);
    EXPECT_NEAR(4.99, values[6], 0.0002);
}

TEST_F(ChannelFixture, ToStringFormatting) {
    channel.run(inputs);
    std::string output = channel.to_string();

    EXPECT_NE(output.find("16-bit"), std::string::npos);
    EXPECT_NE(output.find("Samples: 7"), std::string::npos);
}