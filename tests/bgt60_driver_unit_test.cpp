#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(dummySuite, dummyFunciton)
{
   EXPECT_EQ(5, 5);
}

int main(int argc, char **argv)
{
   ::testing::InitGoogleMock(&argc, argv);

   if (RUN_ALL_TESTS())
   {
   }
   return 0;
}