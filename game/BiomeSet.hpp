#ifndef BIOMESET_HPP
#define BIOMESET_HPP
#include "commons.hpp"

class BiomeSet final {
    public:
        BiomeSet(std::set<int> b, bool reverse=false) : b(b), reverse(reverse) {}
        ~BiomeSet() {}

        bool test(int biome) const {
            return reverse? !b.count(biome) : b.count(biome);
        }
    private:
        const std::set<int> b;
        const bool reverse = false;
};

#endif // BIOMESET_HPP
