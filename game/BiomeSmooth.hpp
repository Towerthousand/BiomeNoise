#ifndef BIOMESMOOTH_HPP
#define BIOMESMOOTH_HPP
#include "BiomeFunction.hpp"

class BiomeSmooth final : public BiomeFunction {
    public:
        BiomeSmooth(std::mt19937* generator, BiomeFunction* base) : BiomeFunction(generator), base(base) {
        }
        virtual ~BiomeSmooth() {
        }

        std::vector<int> getBiomeData(int px, int pz, int sx, int sz) const override {
            int px2 = px - 1;
            int pz2 = pz - 1;
            int sx2 = sx + 2;
            int sz2 = sz + 2;
            std::vector<int> baseData = base->getBiomeData(px2, pz2, sx2, sz2);
            std::vector<int> data(sx * sz);

            for (int z = 0; z < sz; ++z) {
                for (int x = 0; x < sx; ++x) {
                    int left = baseData[x + 0 + (z + 1) * sx2];
                    int right = baseData[x + 2 + (z + 1) * sx2];
                    int top = baseData[x + 1 + (z + 0) * sx2];
                    int bottom = baseData[x + 1 + (z + 2) * sx2];
                    int center = baseData[x + 1 + (z + 1) * sx2];

                    if (left == right && top == bottom) {
                        if (randForPos(2, x + px, z + pz, 0) == 0)
                            center = left;
                        else
                            center = top;
                    }
                    else {
                        if (left == right)
                            center = left;
                        if (top == bottom)
                            center = top;
                    }
                    data[x + z * sx] = center;
                }
            }
            return data;
        }

    private:
        const BiomeFunction* base = nullptr;
};

#endif // BIOMESMOOTH_HPP
