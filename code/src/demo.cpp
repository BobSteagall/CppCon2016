//- demo.cpp - for CppCon talk
//
#ifdef _WIN32
#pragma warning (disable: 4100 4101 4189 4390 4494 4503 4661)
#endif

// #define SEE_MAP_BUG

#include <iostream>

#include <deque>
#include <list>
#include <map>
#include <forward_list>
#include <string>
#include <unordered_map>
#include <vector>

#include "segmented_addressing_model.h"
#include "segmented_private_storage_model.h"
#include "synthetic_pointer_interface.h"
#include "segmented_leaky_allocation_strategy.h"
#include "rhx_allocator.h"

using namespace std;

using test_strategy = segmented_leaky_allocation_strategy<segmented_private_storage_model>;

template<class T> using test_allocator     = rhx_allocator<T, test_strategy>;
template<class C> using test_string        = basic_string<C, char_traits<C>, test_allocator<C>>;
template<class T> using test_fwdlist       = forward_list<T, test_allocator<T>>;
template<class T> using test_list          = list<T, test_allocator<T>>;
template<class T> using test_deque         = deque<T, test_allocator<T>>;
template<class T> using test_vector        = vector<T, test_allocator<T>>;
template<class K, class V> using test_map  = map<K, V, less<K>, test_allocator<pair<K const, V>>>;
template<class K, class V> using test_umap = unordered_map<K, V, hash<K>, equal_to<K>, test_allocator<pair<K const, V>>>;

void test1()
{
    auto    spl = allocate<test_fwdlist<test_string<char>>, test_strategy>();
    char    str[256];

    for (int i = 1719;  i <= 1729;  ++i)
    {
        sprintf(str, "this is very long test string of bollocks #%d for foward_list", i);
        spl->push_front(str);
    }

    cout << endl;
    cout << "************************" << endl;
    cout << "****  TEST FWDLIST  ****" << endl;
    cout << "orignal forward list address is: " << &(*spl) << endl;

    for (auto e : *spl)
    {
        cout << e << endl;
    }

    cout << "******  SWAPPING  ******" << endl;
    test_strategy::swap_buffers();
    cout << "relocated forward list address is: " << &(*spl) << endl;

    for (auto e : *spl)
    {
        cout << e << endl;
    }
}

void test2()
{
    auto    spl = allocate<test_list<test_string<char>>, test_strategy>();
    char    str[256];

    for (int i = 107;  i <= 117;  ++i)
    {
        sprintf(str, "this is very long test string of drivel and dust #%d for list", i);
        spl->push_back(str);
    }

    cout << endl;
    cout << "************************" << endl;
    cout << "*****  TEST LIST  ******" << endl;
    cout << "orignal list is: " << &(*spl) << endl;

    for (auto e : *spl)
    {
        cout << e << endl;
    }

    cout << "******  SWAPPING  ******" << endl;
    test_strategy::swap_buffers();
    cout << "relocated list address is: " << &(*spl) << endl;

    for (auto e : *spl)
    {
        cout << e << endl;
    }
}

void test3()
{
    auto    spd = allocate<test_deque<test_string<char>>, test_strategy>();
    char    str[256];

    for (int i = 67;  i <= 77;  ++i)
    {
        sprintf(str, "this is very long test string of flotsam and jetsam #%d for deque", i);
        spd->push_back(str);
    }

    cout << endl;
    cout << "************************" << endl;
    cout << "*****  TEST DEQUE  *****" << endl;
    cout << "orignal deque address is: " << &(*spd) << endl;

    for (auto e : *spd)
    {
        cout << e << endl;
    }

    cout << "******  SWAPPING  ******" << endl;
    test_strategy::swap_buffers();
    cout << "relocated deque address is: " << &(*spd) << endl;

    for (auto e : *spd)
    {
        cout << e << endl;
    }
}

void test4()
{
    auto    spv = allocate<test_vector<test_string<char>>, test_strategy>();
    char    str[256];

    for (int i = 37;  i <= 47;  ++i)
    {
        sprintf(str, "this is very long test string of dreck and blither #%d for vector", i);
        spv->push_back(str);
    }

    cout << endl;
    cout << "************************" << endl;
    cout << "****  TEST VECTOR  *****" << endl;
    cout << "orignal vector address is: " << &(*spv) << endl;

    for (auto e : *spv)
    {
        cout << e << endl;
    }

    cout << "******  SWAPPING  ******" << endl;
    test_strategy::swap_buffers();
    cout << "relocated vector address is: " << &(*spv) << endl;

    for (auto e : *spv)
    {
        cout << e << endl;
    }
}

void test5()
{
    using demo_map = test_umap<test_string<char>, test_list<test_string<char>>>;

    auto    spmap = allocate<demo_map, test_strategy>();
    auto    spkey = allocate<test_string<char>, test_strategy>();
    auto    spval = allocate<test_string<char>, test_strategy>();

    for (int i = 0;  i < 10;  ++i)
    {
        char    key[512], value[512];

        sprintf(key, "this is test key string %d", i);
        spkey->assign(key);

        for (int j = 1;  j <= 5;  ++j)
        {
            sprintf(value, "this is a very, very, very long test value string %d", i*100+j);
            spval->assign(value);
            (*spmap)[*spkey].push_back(*spval);
        }
    }

    cout << endl;
    cout << "************************" << endl;
    cout << "*****  TEST UMAP  ******" << endl;
    cout << "orignal unordered map address is: " << &(*spmap) << endl;

    for (auto const& e : *spmap)
    {
        cout << e.first << endl;

        for (auto const& v : e.second)
        {
            cout << "    " <<  v << endl;
        }
    }

    cout << "******  SWAPPING  ******" << endl;
    test_strategy::swap_buffers();
    cout << "relocated unordered map address is: " << &(*spmap) << endl;

    for (auto const& e : *spmap)
    {
        cout << e.first << endl;

        for (auto const& v : e.second)
        {
            cout << "    " <<  v << endl;
        }
    }
}

void test6()
{
    using demo_map = test_map<test_string<char>, test_list<test_string<char>>>;

    auto    spmap = allocate<demo_map, test_strategy>();
    auto    spkey = allocate<test_string<char>, test_strategy>();
    auto    spval = allocate<test_string<char>, test_strategy>();

    for (int i = 0;  i < 10;  ++i)
    {
        char    key[512], value[512];

        sprintf(key, "this is test key string %d", i);
        spkey->assign(key);

        for (int j = 1;  j <= 5;  ++j)
        {
            sprintf(value, "this is a very, very, very long test value string %d", i*100+j);
            spval->assign(value);
            (*spmap)[*spkey].push_back(*spval);
        }
    }

    cout << endl;
    cout << "************************" << endl;
    cout << "******  TEST MAP  ******" << endl;
    cout << "orignal map address is: " << &(*spmap) << endl;

    for (auto const& e : *spmap)
    {
        cout << e.first << endl;

        for (auto const& v : e.second)
        {
            cout << "    " <<  v << endl;
        }
    }

    cout << "******  SWAPPING  ******" << endl;
    test_strategy::swap_buffers();
    cout << "relocated map address is: " << &(*spmap) << endl;

    size_t cnt = 0;

    for (auto const& e : *spmap)
    {
        cout << e.first << endl;

        for (auto const& v : e.second)
        {
            cout << "    " <<  v << endl;
        }

        if (++cnt == 20) 
        {
            cout << "ERROR, map iterator caught in loop" << endl;
            break;
        }
    }
}

int main()
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();

    return 0;
}
