#include "gtest/gtest.h"
#include "PropertyIdBuilder.h"

using namespace PJ;
using namespace std;

namespace PropertyIdBuilderTests {
}

using namespace PropertyIdBuilderTests;

TEST(PropertyIdBuilder, BasicTypes)
{
    String value1 = PropertyIdBuilder().Add("name1", "a").Add("name2", "b").Result();
    EXPECT_EQ(String("{\"name1\":\"a\",\"name2\":\"b\"}"), value1);

    String value2 = PropertyIdBuilder().Add("name1", 1).Add("name2", 3.1f).Result();
    EXPECT_EQ(String("{\"name1\":1,\"name2\":3.1}"), value2);
}

TEST(PropertyIdBuilder, CollectionFloat)
{
    std::array<float, 4> values{ 1.1f, 2.2f, 3, 4 };
    String value = PropertyIdBuilder().AddCollection("test", values).Result();

    EXPECT_EQ(String("{\"test\":[1.1,2.2,3,4]}"), value);
}

TEST(PropertyIdBuilder, CollectionString)
{
    std::array<String, 2> values{ "1", "2" };
    String value = PropertyIdBuilder().AddCollection("name", values).Result();

    EXPECT_EQ(String("{\"name\":[\"1\",\"2\"]}"), value);
}

TEST(PropertyIdBuilder, MapIntValue)
{
    std::map<String, int> values{{"one", 1}, {"two", 2}};
    String value = PropertyIdBuilder().AddMap("name", values).Result();

    EXPECT_EQ(String("{\"name\":{\"one\":1,\"two\":2}}"), value);
}

TEST(PropertyIdBuilder, MapStringValue)
{
    std::map<String, String> values{{"one", "1"}, {"two", "2"}};
    String value = PropertyIdBuilder().AddMap("name", values).Result();

    EXPECT_EQ(String("{\"name\":{\"one\":\"1\",\"two\":\"2\"}}"), value);
}
