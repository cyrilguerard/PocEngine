#include "gtest/gtest.h"


TEST(Example, OneEqOne) {
	int v = 1;
	EXPECT_EQ(v, 1);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
