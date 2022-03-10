#include <string_view.hpp>
#include <gtest/gtest.h>
#include <string.h>

TEST(STRING_VIEW_TEST,BASE_TEST){
    const char *str = "123";
    estd::string_view view1(str);
    EXPECT_EQ(view1.length(),strlen(str));
    int n = 0;
    for(auto it : view1){
        EXPECT_EQ(it,str[n++]);
    }

    estd::string_view view2(str);
    EXPECT_EQ(view1,view2);
}