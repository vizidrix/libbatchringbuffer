#pragma once
#include <src/batchringbuffer.h>

#include <gtest/gtest.h>

#include <errno.h>

class Given_a_4_batch_buffer : public testing::Test {
protected:
	brb_buffer * buffer;
	char cancel;

	virtual void SetUp() {
		cancel = 0;
		brb_init_buffer(&buffer, 4, 4, 4);
	}

	virtual void TearDown() {
		cancel = 0;
		brb_free_buffer(&buffer);
	}
};

TEST_F(Given_a_4_batch_buffer, _when_a_zero_sized_claim_is_requested) {
	brb_batch * batch = brb_claim(buffer, 0, &cancel);

	EXPECT_EQ(BRB_CLAIM_PANIC, errno) << "then it should have returned an error";
}

TEST_F(Given_a_4_batch_buffer, _when_a_claim_size_bigger_than_the_buffer_is_requested) {
	brb_batch * batch = brb_claim(buffer, 5, &cancel);

	EXPECT_EQ(BRB_CLAIM_PANIC, errno) << "then it should have returned an error";
}

TEST_F(Given_a_4_batch_buffer, _when_a_valid_single_claim_is_requested) {
	brb_batch * batch = brb_claim(buffer, 1, &cancel);

	EXPECT_EQ(BRB_SUCCESS, errno) << "then it shouldn't have returned an error";
	EXPECT_EQ(0, batch->batch_num) << "then it should have the correct batch num assigned";
	EXPECT_EQ(1, batch->batch_size) << "then it should have the correct batch size";
	EXPECT_EQ(0xFFFFFFFF, batch->seq_num) << "then the seq num should be masked out";
	EXPECT_EQ(0x00000000, batch->group_flags) << "then the group flags should have been reset";
}

TEST_F(Given_a_4_batch_buffer, _when_two_valid_batches_are_claimed) {
	brb_batch * batch1 = brb_claim(buffer, 1, &cancel);
	brb_batch * batch2 = brb_claim(buffer, 1, &cancel);

	EXPECT_EQ(0, batch1->batch_num) << "then it should have assigned batch num zero to the first batch";
	EXPECT_EQ(1, batch2->batch_num) << "then it should have assigned batch num one to the second batch";
}

TEST_F(Given_a_4_batch_buffer, _when_release_is_called_on_unpublished_batch) {
	brb_batch * batch = brb_claim(buffer, 1, &cancel);

	brb_release(buffer, batch);

	EXPECT_EQ(BRB_RELEASE_UNPUBLISHED, errno) << "then it should return a release unpublished error";
}

TEST_F(Given_a_4_batch_buffer, _when_release_is_called_on_already_released_batch) {
	brb_batch * batch = brb_claim(buffer, 1, &cancel);

	brb_publish(buffer, batch);
	brb_release(buffer, batch);
	brb_release(buffer, batch);

	EXPECT_EQ(BRB_RELEASE_UNPUBLISHED, errno) << "then it should return a released unpublished (already released) error";
}

TEST_F(Given_a_4_batch_buffer, _when_release_is_called_out_of_order) {
	brb_batch * batch1 = brb_claim(buffer, 1, &cancel);
	brb_batch * batch2 = brb_claim(buffer, 1, &cancel);

	brb_publish(buffer, batch2);
	brb_release(buffer, batch2);

	EXPECT_EQ(BRB_RELEASE_OVERFLOW, errno) << "then it should return a release overflow error";
}

TEST_F(Given_a_4_batch_buffer, _when_claiming_batches_that_were_previously_released) {
	brb_batch * batch1 = brb_claim(buffer, 1, &cancel);
	brb_batch * batch2 = brb_claim(buffer, 1, &cancel);
	brb_batch * batch3 = brb_claim(buffer, 1, &cancel);
	brb_batch * batch4 = brb_claim(buffer, 1, &cancel);
	/* Buffer should be fully allocated now so without releasing, next claim will block */

	brb_publish(buffer, batch1);
	brb_release(buffer, batch1);

	/* Need to figure out how to make sure this doesn't hang the tests */
	brb_batch * batch5 = brb_claim(buffer, 1, &cancel);


}


/* when all batches are allocated then it should block... how to trigger cancel in C? */
/* when batches share a cancel token they should both return on cancel */


