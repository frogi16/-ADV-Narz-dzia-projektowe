#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <array>
#include <numeric>
#include <random>
#include <algorithm>

#include "CircularBuffer.h"

using namespace ::testing;

constexpr size_t bufferSize{ 5 };

class CreateAndCompletelyFillBufferOfDoublesTest : public Test
{
protected:

	void SetUp() override
	{
		EXPECT_EQ(buffer.getAllocatedSize(), bufferSize);
		EXPECT_EQ(buffer.getSize(), 0);

		std::iota(vals.begin(), vals.end(), -3);

		for (size_t i = 0; i < vals.size(); i++)
		{
			buffer.add(vals[i]);
			EXPECT_EQ(buffer.getSize(), i + 1);
		}

		EXPECT_EQ(buffer.getAllocatedSize(), bufferSize);
		EXPECT_EQ(buffer.getSize(), bufferSize);
	}

	void TearDown() override
	{
		EXPECT_EQ(buffer.getAllocatedSize(), bufferSize);
	}

	CircularBuffer<double, bufferSize> buffer;
	std::array<double, bufferSize> vals;

	std::random_device rd;
	std::mt19937 gen{ rd() };

	size_t generateRandomValue(size_t min, size_t max)
	{
		std::uniform_int_distribution<> distrib(min, max);
		return distrib(gen);
	}
};

TEST_F(CreateAndCompletelyFillBufferOfDoublesTest, RetrieveElementsInOrderOfAdding)
{
	for (auto &v : vals)
		ASSERT_EQ(buffer.pop(), v);
}

TEST_F(CreateAndCompletelyFillBufferOfDoublesTest, RetrieveTooManyElements_shouldThrow)
{
	for (auto &v : vals)
		buffer.pop();

	ASSERT_THROW(buffer.pop(), RetrieveFromEmptyBufferException);
}
TEST_F(CreateAndCompletelyFillBufferOfDoublesTest, AddAdditionalElement_shouldNotFail)
{
	ASSERT_NO_THROW(buffer.add(0));
}

TEST_F(CreateAndCompletelyFillBufferOfDoublesTest, AddAdditionalElement_shouldOverwriteOldestAndReturnLast)
{
	buffer.add(0);

	for (size_t i = 1; i < vals.size(); i++)
		ASSERT_EQ(buffer.pop(), vals[i]);

	ASSERT_EQ(buffer.pop(), 0);
}

TEST_F(CreateAndCompletelyFillBufferOfDoublesTest, AddTooManyElements_shouldOverwriteAllNeededAndReturnInOrderOfAddition)
{
	std::vector<double> additionalElements(generateRandomValue(20, 50));
	std::generate(additionalElements.begin(), additionalElements.end(), [this](){ return generateRandomValue(0, 100) / 3.14f; });
	for (auto &v : additionalElements)
		buffer.add(v);

	EXPECT_EQ(buffer.getSize(), bufferSize);

	for (size_t i = additionalElements.size() - bufferSize; i < additionalElements.size(); i++)
		ASSERT_EQ(buffer.pop(), additionalElements[i]);

	EXPECT_EQ(buffer.getSize(), 0);
}

TEST(ParametrizedBufferTest, UseForStrings_shouldProvideAllFunctionalities)
{
	CircularBuffer<std::string, bufferSize> buffer;
	for (size_t i = 0; i < 33; i++)
		buffer.add(std::string("Test string" + std::to_string(i)));

	for (size_t i = 28; i < 33; i++)
		ASSERT_EQ(buffer.pop(), std::string("Test string" + std::to_string(i)));

	EXPECT_EQ(buffer.getSize(), 0);
}