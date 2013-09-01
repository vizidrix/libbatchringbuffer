#pragma once
#include <src/batchringbuffer.h>

#include <gtest/gtest.h>

class Given_no_state : public testing::Test {
protected:
	brb_buffer * buffer;
};

TEST_F(Given_no_state, _when_a_valid_buffer_is_created) {
	brb_buffer_info * info;

	brb_init_buffer(&buffer, 8, 8, 16);
	brb_get_info(buffer, &info);

	EXPECT_EQ(8, info->batch_buffer_size) << "then it should have recorder the batch size";
	EXPECT_EQ(8, info->data_buffer_size) << "then it should have recorded the data size";
	EXPECT_EQ(16, info->entry_size) << "then it should have recorded the entry size";
}

TEST_F(Given_no_state, _when_a_buffer_is_created_with_invalid_sizes) {
	brb_buffer_info * info;

	brb_init_buffer(&buffer, 6, 17, 10);
	brb_get_info(buffer, &info);

	EXPECT_EQ(8, info->batch_buffer_size) << "then batch buffer size should have been rounded to next 2s complement";
	EXPECT_EQ(32, info->data_buffer_size) << "then data buffer size should have been rounded to next 2s complement";
}

TEST_F(Given_no_state, _when_a_buffer_is_closed_twice) {
	brb_init_buffer(&buffer, 4, 4, 4);
	brb_free_buffer(&buffer);
	brb_free_buffer(&buffer);
}
