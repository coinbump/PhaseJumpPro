#include "gtest/gtest.h"
#include "ViewSizeProposal.h"

using namespace std;
using namespace PJ;

namespace ViewSizeProposalTests {
}

using namespace ViewSizeProposalTests;

TEST(ViewSizeProposal, ReplacingNullOptionals) {
    ViewSizeProposal sut;
    EXPECT_FALSE(sut.width);
    EXPECT_FALSE(sut.height);
    
    sut = sut.ReplacingNullOptionals(10);
    EXPECT_EQ(10, *sut.width);
    EXPECT_EQ(10, *sut.height);
}
