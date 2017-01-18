#ifndef BIOMECONST_HPP
#define BIOMECONST_HPP
#include "BiomeFunction.hpp"

class BiomeConst final : public BiomeFunction {
    public:
        BiomeConst(std::mt19937* generator, int biome) : BiomeFunction(generator), biome(biome) {
        }

        virtual ~BiomeConst() {
        }

        std::vector<int> getBiomeData(int px, int pz, int sx, int sz) const override {
            (void) px;
            (void) pz;
            return std::vector<int>(sx*sz, biome);
        }
    private:
        const int biome = 0;
};

#endif // BIOMECONST_HPP
