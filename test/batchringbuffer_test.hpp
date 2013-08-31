#pragma once
#include <src/batchringbuffer.h>

#include <gtest/gtest.h>

TEST(BatchRingBufferTest, Should_not_pass)
{
	brb_buffer * buffer;
	brb_init_buffer(&buffer, 1000, 1, 1024);
	char cancel = 0;
	brb_batch * batch = brb_claim(buffer, 1, &cancel);
	brb_publish(buffer, batch);
	brb_release(buffer, batch);

	/*EXPECT_NE(test, test);*/
	printf("OUTPUT!!!\n");
	EXPECT_EQ(1, batch->batch_size) << "Batch size was " << batch->batch_size;
}

TEST(BatchRingBufferTest, Should_not_pass2)
{
	brb_buffer * buffer;
	brb_init_buffer(&buffer, 1000, 1, 1024);
	char cancel = 0;
	brb_batch * batch = brb_claim(buffer, 1, &cancel);
	brb_publish(buffer, batch);
	brb_release(buffer, batch);

	/*EXPECT_NE(test, test);*/
	printf("OUTPUT!!!\n");
	EXPECT_EQ(1, batch->batch_size) << "Batch size was " << batch->batch_size;
}

TEST(BatchRingBufferTest, Should_not_pass3)
{
	brb_buffer * buffer;
	brb_init_buffer(&buffer, 1000, 1, 1024);
	char cancel = 0;
	brb_batch * batch = brb_claim(buffer, 1, &cancel);
	brb_publish(buffer, batch);
	brb_release(buffer, batch);

	/*EXPECT_NE(test, test);*/
	printf("OUTPUT!!!\n");
	EXPECT_EQ(1, batch->batch_size) << "Batch size was " << batch->batch_size;
}