#include <algorithm>
#include <iostream>
#include "hash/map.h"
#include "hash/set.h"
#include "hash/unordered_map.h"
#include "hash/unordered_set.h"

template<std::size_t N>
struct EquateModulo
{
    bool operator()(std::size_t lhs, std::size_t rhs) const
    {
        return (lhs % N) == (rhs % N);
    }
};

template<std::size_t N>
struct HashModulo
{
    std::size_t operator()(std::size_t value) const
    {
        return value % N;
    }
};

template<std::size_t N>
struct CompareModulo
{
    bool operator()(std::size_t lhs, std::size_t rhs) const
    {
        return (lhs % N) < (rhs % N);
    }
};

template<typename Set>
bool std_equal(Set const& lhs, Set const& rhs)
{
    // unordered comparison as per standard
    if (lhs.size() != rhs.size())
        return false;

    for(auto it = lhs.cbegin(); it != lhs.cend(); ) {
        auto const& key = *it;
        auto l_range = lhs.equal_range(key);
        auto r_range = rhs.equal_range(key);

        if (!std::is_permutation(l_range.first, l_range.second, r_range.first, r_range.second))
            return false;
        it = l_range.second;
    }
    return true;
}

template<typename Set>
bool test_sets()
{
    Set set1 = {1, 2};
    Set set2 = {21, 12};
    hash::shash<> h;

    return (set1 == set2) == (h(set1) == h(set2));
}

template<typename Map>
bool test_maps()
{
    Map set1 = {{1, 1}, {2, 2}};
    Map set2 = {{21, 1}, {12, 2}};
    hash::shash<> h;

    return (set1 == set2) == (h(set1) == h(set2));
}

int main()
{
    using Set = std::set<std::size_t, CompareModulo<10>>;
    using Map = std::map<std::size_t, int, CompareModulo<10>>;
    using MultiSet = std::multiset<std::size_t, CompareModulo<10>>;
    using MultiMap = std::multimap<std::size_t, int, CompareModulo<10>>;
    using UnorderedSet = std::unordered_set<std::size_t, HashModulo<10>, EquateModulo<10>>;
    using UnorderedMap = std::unordered_map<std::size_t, int ,HashModulo<10>, EquateModulo<10>>;
    using UnorderedMultiSet = std::unordered_multiset<std::size_t, HashModulo<10>, EquateModulo<10>>;
    using UnorderedMultiMap = std::unordered_multimap<std::size_t, int, HashModulo<10>, EquateModulo<10>>;

    std::cout << std::boolalpha;
    std::cout << "Sets are equal: " << test_sets<Set>() << std::endl;
    std::cout << "MultiSets are equal: " << test_sets<MultiSet>() << std::endl;
    std::cout << "UnorderedSets are equal: " << test_sets<UnorderedSet>() << std::endl;
    std::cout << "UnorderedMultiSets are equal: " << test_sets<UnorderedMultiSet>() << std::endl;

    std::cout << std::endl;
    std::cout << "Maps are equal: " << test_maps<Map>() << std::endl;
    std::cout << "MultiMaps are equal: " << test_maps<MultiMap>() << std::endl;
    std::cout << "UnorderedMaps are equal: " << test_maps<UnorderedMap>() << std::endl;
    std::cout << "UnorderedMultiMaps are equal: " << test_maps<UnorderedMultiMap>() << std::endl;

    return 0;
}
