#include <algorithm>
#include <set.hpp>
#include <vector.hpp>
#include <gtest/gtest.h>
#include "class.hpp"

TEST(SET_TEST,CONSTRUCTOR_TEST){
    //constructor
    { 
       estd::set<int> s;
       
       estd::set<int> s1{1,2,3,4,5};
      
       auto compare = [](int lhs,int rhs){
           return lhs < rhs;
       };

       estd::set<int,decltype(compare)> s2(compare);

       estd::vector<int> v{3,2,1};

       estd::set<int> s3(v.begin(),v.end());
       int n = 1;
       for(auto it : s3){
           EXPECT_EQ(it,n++);
       }

    }

    //copy_construcor
    estd::set<int> s{1,2,3,4};
    estd::set<int> s1(s);

    EXPECT_FALSE(s.empty());
    EXPECT_FALSE(s1.empty());
    EXPECT_EQ(s1.size(),4);
    EXPECT_EQ(s.size(),4);
    int n = 1;
    for(auto it : s){
        EXPECT_EQ(it,n++);
    }
    n = 1;
    for(auto it : s1){
        EXPECT_EQ(it,n++);
    }
    
    //move_constructor
    estd::set<int> move_set(estd::move(s));

    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(),0);

    EXPECT_FALSE(move_set.empty());
    EXPECT_EQ(move_set.size(),4);

    n = 1;
    for(auto it : move_set){
        EXPECT_EQ(it,n++);
    }

    //copy operator=
    estd::set<int> copy_operator_set;
    copy_operator_set = move_set;


    EXPECT_FALSE(copy_operator_set.empty());
    EXPECT_FALSE(move_set.empty());
    EXPECT_EQ(copy_operator_set.size(),4);
    EXPECT_EQ(move_set.size(),4);
    n = 1;
    for(auto it : copy_operator_set){
        EXPECT_EQ(it,n++);
    }
    n = 1;
    for(auto it : move_set){
        EXPECT_EQ(it,n++);
    }
    
    //move operator=

    estd::set<int> move_operator_set;
    move_operator_set = estd::move(copy_operator_set);
    
    EXPECT_TRUE(copy_operator_set.empty());
    EXPECT_EQ(copy_operator_set.size(),0);

    EXPECT_FALSE(move_operator_set.empty());
    EXPECT_EQ(move_operator_set.size(),4);

    n = 1;
    for(auto it : move_operator_set){
        EXPECT_EQ(it,n++);
    }

}

TEST(SET_TEST,INTERFACE_TEST){
    estd::set<int> s;

    //insert
    {
        auto &&[pos,flag] = s.insert(1);
        EXPECT_TRUE(flag);
        EXPECT_EQ(*pos,1);
    }
    {
        auto &&[pos,flag] = s.insert(1);
        EXPECT_FALSE(flag);
    }
    EXPECT_EQ(s.size(),1);

    estd::vector<int> v{2,3,4};

    s.insert(v.begin(),v.end());

    EXPECT_EQ(s.size(),4);
    int n = 1;
    for(auto it : s){
        EXPECT_EQ(it,n++);
    }
    //clear
    s.clear();
    EXPECT_TRUE(s.empty());
  

    for(int i = 0;i < 1000; i++){
        s.insert(i);
    }
    EXPECT_EQ(s.size(),1000);

    //find
    auto it = s.find(5);
    ASSERT_TRUE(it != s.end());
    EXPECT_EQ(*it,5);

    it = s.find(-1);
    ASSERT_TRUE(it == s.end());

    //lower_bound
    it = s.lower_bound(12);
    ASSERT_TRUE(it != s.end());
    EXPECT_EQ(*it,12);

    it = s.lower_bound(100000);
    ASSERT_TRUE(it == s.end());
    //upper_bound
    it = s.upper_bound(12);
    ASSERT_TRUE(it != s.end());
    EXPECT_EQ(*it,13);
    
    it = s.upper_bound(100000);
    ASSERT_TRUE(it == s.end());

    //equal_range
    {
        auto [first,last] = s.euqal_range(2);
        EXPECT_EQ(*first,2);
    }
    {
        auto [first,last] = s.euqal_range(200000);
        ASSERT_TRUE(first == s.end());
        ASSERT_TRUE(last == s.end());
    }

    //count

    n = s.count(1);
    EXPECT_EQ(n,1);

    n = s.count(-1);
    EXPECT_EQ(n,0);
    
    //iterator
    it = s.begin();
    EXPECT_EQ(*it,0);
    auto it1 = it++;
    EXPECT_EQ(*it1,0);
    EXPECT_EQ(*it,1);

    ++it;
    EXPECT_EQ(*it,2);

    auto it2 = it--;
    EXPECT_EQ(*it2,2);
    EXPECT_EQ(*it,1);

    --it;
    EXPECT_EQ(*it,0);
    ++it2;
    s.erase(it,it2);
    EXPECT_EQ(*s.begin(),3);

    s.erase(s.begin(),s.end());
    EXPECT_EQ(s.size(),0);
    EXPECT_TRUE(s.empty());
}

TEST(SET_TEST,OBJECT_TEST){
    using namespace estd::test;
    
    auto compare = [](const A &lhs,const A&rhs){
        return *lhs < *rhs;
    };

    estd::set<A,decltype(compare)> s(compare);
    
    s.emplace(1);
    EXPECT_EQ(**s.begin(),1);
    s.clear();

    A a(100);
    s.insert(estd::move(a));
    ASSERT_TRUE(a.value == nullptr);
    EXPECT_EQ(**s.begin(),100);
}

TEST(MULTISET_TEST,CONSTRUCTOR_TEST){
    //constructor
    { 
       estd::multiset<int> s;
       
       estd::multiset<int> s1{1,2,3,4,5};
      
       auto compare = [](int lhs,int rhs){
           return lhs < rhs;
       };

       estd::multiset<int,decltype(compare)> s2(compare);

       estd::vector<int> v{3,2,1};

       estd::multiset<int> s3(v.begin(),v.end());
       int n = 1;
       for(auto it : s3){
           EXPECT_EQ(it,n++);
       }

    }

    //copy_construcor
    estd::multiset<int> s{1,2,3,4};
    estd::multiset<int> s1(s);

    EXPECT_FALSE(s.empty());
    EXPECT_FALSE(s1.empty());
    EXPECT_EQ(s1.size(),4);
    EXPECT_EQ(s.size(),4);
    int n = 1;
    for(auto it : s){
        EXPECT_EQ(it,n++);
    }
    n = 1;
    for(auto it : s1){
        EXPECT_EQ(it,n++);
    }
    
    //move_constructor
    estd::multiset<int> move_set(estd::move(s));

    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(),0);

    EXPECT_FALSE(move_set.empty());
    EXPECT_EQ(move_set.size(),4);

    n = 1;
    for(auto it : move_set){
        EXPECT_EQ(it,n++);
    }

    //copy operator=
    estd::multiset<int> copy_operator_set;
    copy_operator_set = move_set;


    EXPECT_FALSE(copy_operator_set.empty());
    EXPECT_FALSE(move_set.empty());
    EXPECT_EQ(copy_operator_set.size(),4);
    EXPECT_EQ(move_set.size(),4);
    n = 1;
    for(auto it : copy_operator_set){
        EXPECT_EQ(it,n++);
    }
    n = 1;
    for(auto it : move_set){
        EXPECT_EQ(it,n++);
    }
    
    //move operator=

    estd::multiset<int> move_operator_set;
    move_operator_set = estd::move(copy_operator_set);
    
    EXPECT_TRUE(copy_operator_set.empty());
    EXPECT_EQ(copy_operator_set.size(),0);

    EXPECT_FALSE(move_operator_set.empty());
    EXPECT_EQ(move_operator_set.size(),4);

    n = 1;
    for(auto it : move_operator_set){
        EXPECT_EQ(it,n++);
    }
    
}

TEST(MULTISET_TEST,INTERFACE_TEST){
    estd::multiset<int> s;

    for(int i = 0; i < 100; i++)
       s.insert(1);

    EXPECT_EQ(s.size(),100);

    size_t n = s.count(1);

    EXPECT_EQ(n,100);
    
    auto [first,last] = s.euqal_range(1);
    ASSERT_TRUE(last == s.end());
    n = 0;
    
    while(first != last){
        EXPECT_EQ(*first,1);
        n++;
        first++;
    }

    EXPECT_EQ(n,100);

    n = s.erase(1);
    EXPECT_EQ(n,100);

    EXPECT_TRUE(s.empty());
}

TEST(MULTISET_TEST,OBJECT_TEST){
    using namespace estd::test;
    
    auto compare = [](const A &lhs,const A&rhs){
        return *lhs < *rhs;
    };

    estd::multiset<A,decltype(compare)> s(compare);
    
    s.emplace(1);
    EXPECT_EQ(**s.begin(),1);
    s.clear();

    A a(100);
    s.insert(estd::move(a));
    ASSERT_TRUE(a.value == nullptr);
    EXPECT_EQ(**s.begin(),100);
}

