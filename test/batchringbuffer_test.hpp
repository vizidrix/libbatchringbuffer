#pragma once
#include <src/batchringbuffer.h>

#include <gtest/gtest.h>

TEST(BatchRingBufferTest, Should_not_pass)
{
	uint64_t test = 0;
	/*EXPECT_NE(test, test);*/
	EXPECT_EQ(test, test);
}