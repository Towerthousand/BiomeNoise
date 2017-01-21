#ifndef BIOMEOUTLINE_HPP
#define BIOMEOUTLINE_HPP
#include "BiomeFunction.hpp"

class BiomeOutline final : public BiomeFunction {
    public:
        BiomeOutline(std::mt19937* generator, BiomeFunction* base, BiomeSet biomes, BiomeSet nextTo, int outlineBiome, bool outer = false) :
            BiomeFunction(generator), base(base), biomes(biomes), nextTo(nextTo), outlineBiome(outlineBiome), outer(outer) {
            VBE_ASSERT_SIMPLE(base != nullptr);
        }

        virtual ~BiomeOutline() {
        }

        std::vector<int> getBiomeData(int px, int pz, int sx, int sz) const override {
            std::vector<int> baseData = base->getBiomeData(px - 1, pz - 1, sx + 2, sz + 2);
            std::vector<int> result(sx * sz);

            for (int z = 0; z < sz; ++z) {
                for (int x = 0; x < sx; ++x) {
                    int center = baseData[x + 1 + (z + 1) * (sx + 2)];
                    int bottom = baseData[x + 1 + (z + 1 - 1) * (sx + 2)];
                    int right = baseData[x + 1 + 1 + (z + 1) * (sx + 2)];
                    int left = baseData[x + 1 - 1 + (z + 1) * (sx + 2)];
                    int top = baseData[x + 1 + (z + 1 + 1) * (sx + 2)];

                    if(outer) {
                        if (!biomes.test(center)
                            && (biomes.test(bottom) || biomes.test(right) || biomes.test(left) || biomes.test(top))
                            && nextTo.test(bottom) && nextTo.test(right) && nextTo.test(left) && nextTo.test(top))
                            result[x+z*sx] = outlineBiome;
                        else
                            result[x+z*sx] = center;
                    }
                    else {
                        if (biomes.test(center)
                            && (bottom != center || right != center || left != center || top != center)
                            && nextTo.test(bottom) && nextTo.test(right) && nextTo.test(left) && nextTo.test(top))
                            result[x+z*sx] = outlineBiome;
                        else
                            result[x+z*sx] = center;
                    }
                }
            }
            return result;
        }

    private:
        const BiomeFunction* base = nullptr;
        const BiomeSet biomes;
        const BiomeSet nextTo;
        const int outlineBiome = 0;
        const bool outer = false;
};

#endif // BIOMEOUTLINE_HPP
