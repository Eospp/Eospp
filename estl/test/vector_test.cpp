#include <gtest/gtest.h>
#include <vector.hpp>
#include <memory.hpp>
#include <climits>

TEST(VECTOR_TEST,CONSTRUTOR_TEST)
{
    estd::vector<int> v1;
    EXPECT_TRUE(v1.empty());
    EXPECT_EQ(v1.size(),0);
    EXPECT_EQ(v1.capacity(),0);

    estd::vector<int> v2(5);
    EXPECT_FALSE(v2.empty());
    EXPECT_EQ(v2.size(),5);
    EXPECT_EQ(v2.capacity(),5);

    for(int i = 0;i < 5;i++)
    {   
        v2[i] = i;
    }

    estd::vector<int> v3(estd::move(v2));

    EXPECT_TRUE(v2.empty());
    EXPECT_EQ(v2.size(),0);
    EXPECT_EQ(v2.capacity(),0);
    EXPECT_EQ(v2.data(),nullptr);

    EXPECT_FALSE(v3.empty());
    EXPECT_EQ(v3.size(),5);
    
    for(int i = 0;i < 5; i++)
    {
        EXPECT_EQ(v3[i],i);
    }
    
    estd::vector<int> v4(v3);

    EXPECT_FALSE(v4.empty());
    EXPECT_EQ(v4.capacity(),5);
    EXPECT_EQ(v4.size(),5);
    for(int i = 0;i < 5; i++)
    {
        EXPECT_EQ(v4[i],i);
    }


    estd::vector<int> v5;
    v5 = v4;

    EXPECT_FALSE(v5.empty());
    EXPECT_EQ(v5.capacity(),5);
    EXPECT_EQ(v5.size(),5);
    for(int i = 0;i < 5; i++)
    {
        EXPECT_EQ(v5[i],i);
    }


    estd::vector<int> v6;
    v6 = estd::move(v5);

    EXPECT_TRUE(v5.empty());
    EXPECT_EQ(v5.capacity(),0);
    EXPECT_EQ(v5.size(),0);

    EXPECT_FALSE(v6.empty());
    EXPECT_EQ(v6.capacity(),5);
    EXPECT_EQ(v6.size(),5);
    for(int i = 0;i < 5; i++)
    {
        EXPECT_EQ(v6[i],i);
    }

    estd::vector<int> v7(10);

    for(int i = 0;i < 10; i++)
       EXPECT_EQ(v7[i],0);
    
    estd::vector<int> v8(10,7);

    for(int i = 0;i < 10;i++)
      EXPECT_EQ(v8[i],7);
}

TEST(VECTOR_TEST,INTERFACE_TEST)
{
    estd::vector<int> v;
    //push_back
    for(int i = 0;i < 1000; i++)
      v.push_back(i);

    for(int i = 0;i < 1000; i++)
      EXPECT_EQ(v[i],i);

    estd::vector<int>().swap(v);
    //emplace_back
    for(int i = 0;i < 1000; i++)
      v.emplace_back(i);
    
    for(int i = 0;i < 1000; i++)
      EXPECT_EQ(v[i],i);
    //push_front
    estd::vector<int>().swap(v);

    for(int i = 0; i < 1000; i++)
      v.push_front(i);

    for(int i = 0; i < 1000; i++)
      EXPECT_EQ(v[999 - i],i);
    
    estd::vector<int>().swap(v);
    //pop_back
    for(int i = 0; i < 10; i++)
      v.push_back(i);
    
    for(int i = 0;i < 5; i++)
      v.pop_back();
    
    EXPECT_EQ(v.size(),5);

    for(int i = 0;i < 5; i++)
      EXPECT_EQ(v[i],i);

    //pop_front
    for(int i = 0;i < 3; i++)
      v.pop_front();
    
    for(int i = 0; i < 2; i++)
      EXPECT_EQ(v[i],3 + i);
}

class A
{
public:
      A() : p(new int(5))
      {
      }

      A(int v) : p(new int(v))
      {

      }
      A(const A& rhs) : p(new int(*rhs.p))
      {

      }
      int operator*()
      {
          return p ? *p : INT_MIN;
      }
      ~A() 
      { 
          delete p;
      }
private:
     int *p;
};


TEST(VECTOR_TEST,OBJECT_TEST)
{
    estd::vector<A> v;
    int n = 1000;
    for(int i = 0;i < n; i++)
        v.emplace_back(i);
    
    for(int i = 0;i < n; i++)
         EXPECT_EQ(*v[i],i);
    
    estd::vector<A> v1(v);
    EXPECT_EQ(v1.size(),n);

    for(int i = 0;i < n; i++)
      EXPECT_EQ(*v[i],i);
    
    bool flag = false;
    auto deleter = [&flag](int *p)
    {
        flag = true;
        delete p;
    };

    estd::vector<estd::shared_ptr<int>> v2;

    v2.emplace_back(new int(5),deleter);

    v2.pop_back();

    EXPECT_TRUE(flag);

    flag = false;
    v2.emplace_back(new int(5),deleter);

    v2.pop_front();
    EXPECT_TRUE(flag);
}

TEST(VECOTR_TEST,ITERATOR_TEST)
{
    estd::vector<int> v(5);

    for(int i = 1;i <= 5;i++)
       v[i - 1] = i;

    int i = 1;
    for(auto it = v.begin(); it != v.end(); it++,i++)
        EXPECT_EQ(*it,i);
    
    estd::vector<int>::iterator it = v.begin();

    v.insert(it,5);

    EXPECT_EQ(v.front(),5);
    
    it = v.erase(v.begin());

    EXPECT_EQ(v.front(),1);
    EXPECT_EQ(*it,1);

    it = v.erase(v.begin(),v.begin() + 2);
    EXPECT_EQ(*it,3);

    v.erase(v.begin(),v.end());
    EXPECT_TRUE(v.empty());
     
}