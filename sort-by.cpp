#include <algorithm>
#include <tuple>
#include <vector>
#include <iostream>
#include <iterator>

// Returns a sorted sequence of the items in collection,
// where the sort order is determined by comparing keyFn(item).
// keyFn should take const T::value_type& as it's argument and
// return a std::tuple containing types that work with the < operator
template<typename T, typename KeyFn>
T sortBy(KeyFn keyFn, const T& collection) {
    // Map the collection items to tuples of keys
    using CollectionType = typename T::value_type;
    using Keys = typename std::invoke_result<KeyFn, const CollectionType&>::type;
    using Map = std::vector<std::tuple<Keys, CollectionType>>;
    using TupleType = typename Map::value_type;

    Map mappings;
    for (const auto& item : collection) {
        mappings.emplace_back(std::make_tuple(keyFn(item), item));
    }

    std::sort(std::begin(mappings), std::end(mappings),
        [](const TupleType& a, const TupleType& b) -> bool {
            return std::get<0>(a) < std::get<0>(b);
        });

    T result;
    for (const auto& tuple : mappings) {
        result.emplace_back(std::get<1>(tuple));
    }

    return result;
}

struct S {
    int p;
    int q;
    int r;
};

int main() {
    std::vector<S> coll = {
        S{1, 1, 3},
        S{1, 2, 2},
        S{0, 1, 1},
        S{1, 1, 1},
        S{1, 2, 1}
    };

    auto sorted = sortBy(
        [](const S& a) {
            // Sort by p then r then q
            return std::make_tuple(a.p, a.r, a.q);
        },
        coll);

    for (const auto& item : sorted) {
        std::cout << item.p << ' ' << item.q << ' ' << item.r << '\n';
    }

    return 0;
}
