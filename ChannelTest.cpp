// ChannelTest.cpp
#include <gtest/gtest.h>
#include "Channel.h"
#include "Adc.h"

class ChannelFixture : public ::testing::Test {
protected:
    ADC<short, 1024>* adc;
    Channel* channel;

    void SetUp() override {
        adc = new ADC<short, 1024>();
        adc->bits = 10;
        adc->clock_rate = std::chrono::milliseconds(10);
        adc->rangeMin = 0.0;
        adc->rangeMax = 3.3;
        adc->sensitivity = 1.0;
        adc->trigger_level = 1.65;
        channel = new Channel(*adc, 0);
    }

    void TearDown() override {
        delete channel;
        delete adc;
    }
};

TEST_F(ChannelFixture, ConstructorInitializesCorrectly) {
    EXPECT_NO_THROW(Channel(*adc, 0));
}

TEST_F(ChannelFixture, CopyConstructorWorks) {
    Channel copy(*channel);
    EXPECT_NO_THROW(copy.run());
}

TEST_F(ChannelFixture, RunSetsRunningState) {
    channel->run();
    EXPECT_NO_THROW(channel->get_values());
}

TEST_F(ChannelFixture, GetValuesReturnsEmptyWhenNotRunning) {
    std::vector<double> values = channel->get_values();
    EXPECT_TRUE(values.empty());
}

TEST_F(ChannelFixture, GetValuesReturnsValuesWhenRunning) {
    channel->run();
    std::vector<double> inputs = { 0.0, 1.0, 2.0, 3.3 };
    adc->read_input(inputs);
    std::vector<double> values = channel->get_values();
    EXPECT_EQ(values.size(), 4);
}

TEST_F(ChannelFixture, ToStringReturnsNotRunningWhenNotRunning) {
    std::string result = channel->to_string();
    EXPECT_EQ(result, "Channel 0: Not running");
}

TEST_F(ChannelFixture, ToStringReturnsRunningWhenRunning) {
    channel->run();
    std::string result = channel->to_string();
    EXPECT_EQ(result.find("Running"), 0);
}

TEST_F(ChannelFixture, ConvertZeroVoltageReturnsZero) {
    EXPECT_EQ(adc->convert(0.0), 0);
}

TEST_F(ChannelFixture, ConvertMaxVoltageReturnsMaxValue) {
    EXPECT_EQ(adc->convert(3.3), 1023);
}

TEST_F(ChannelFixture, ConvertMidVoltageReturnsMidValue) {
    EXPECT_EQ(adc->convert(1.65), 511);
}

TEST_F(ChannelFixture, ConvertBelowMinReturnsZero) {
    EXPECT_EQ(adc->convert(-1.0), 0);
}

TEST_F(ChannelFixture, ConvertAboveMaxReturnsMax) {
    EXPECT_EQ(adc->convert(5.0), 1023);
}

TEST_F(ChannelFixture, ReadInputTriggersOnThreshold) {
    std::vector<double> inputs = { 1.0, 1.7, 2.0 };
    size_t count = adc->read_input(inputs);
    EXPECT_EQ(count, 2);
    EXPECT_EQ(adc->samples.size(), 2);
}

TEST_F(ChannelFixture, ReadInputNoTriggerBelowThreshold) {
    std::vector<double> inputs = { 0.5, 1.0, 1.6 };
    size_t count = adc->read_input(inputs);
    EXPECT_EQ(count, 0);
    EXPECT_TRUE(adc->samples.empty());
}

TEST_F(ChannelFixture, ValueReturnsCorrectForSample) {
    std::vector<double> inputs = { 1.65 };
    adc->read_input(inputs);
    EXPECT_DOUBLE_EQ(adc->value(0), 1.65);
}

TEST_F(ChannelFixture, ValueReturnsZeroForInvalidIndex) {
    EXPECT_DOUBLE_EQ(adc->value(999), 0.0);
}

TEST_F(ChannelFixture, ValuesReturnsAllSamples) {
    std::vector<double> inputs = { 0.0, 1.65, 3.3 };
    adc->read_input(inputs);
    std::vector<double> result = adc->values();
    EXPECT_EQ(result.size(), 3);
    EXPECT_DOUBLE_EQ(result[0], 0.0);
    EXPECT_DOUBLE_EQ(result[1], 1.65);
    EXPECT_DOUBLE_EQ(result[2], 3.3);
}

TEST_F(ChannelFixture, ValuesReturnsEmptyWhenNoSamples) {
    std::vector<double> result = adc->values();
    EXPECT_TRUE(result.empty());
}

TEST_F(ChannelFixture, ToStringReturnsNonEmpty) {
    std::vector<double> inputs = { 1.65, 2.5 };
    adc->read_input(inputs);
    std::string result = adc->to_string();
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.find("ADC samples:"), 0);
}