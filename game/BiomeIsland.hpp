#ifndef BIOMEISLAND_HPP
#define BIOMEISLAND_HPP
#include "BiomeFunction.hpp"

class BiomeIsland final : public BiomeFunction {
    public:
        BiomeIsland(std::mt19937* generator, BiomeFunction* base) : BiomeFunction(generator), base(base) {
        }
        virtual ~BiomeIsland() {
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
                    int topLeft     = baseData[x + 0 + (z + 0) * sx2];
                    int topRight    = baseData[x + 2 + (z + 0) * sx2];
                    int bottomLeft  = baseData[x + 0 + (z + 2) * sx2];
                    int bottomRight = baseData[x + 2 + (z + 2) * sx2];
                    int center      = baseData[x + 1 + (z + 1) * sx2];

                    //ocean surrounded by some other biome
                    if (center == 0 && (topLeft != 0 || topRight != 0 || bottomLeft != 0 || bottomRight != 0)) {
                        int max = 1;
                        int r = 1;

                        //Choose randomly from the 4 values one that's not 0
                        if (    topLeft != 0 && randForPos(max++, x + px, z + pz, 0) == 0) r = topLeft;
                        if (   topRight != 0 && randForPos(max++, x + px, z + pz, 1) == 0) r = topRight;
                        if ( bottomLeft != 0 && randForPos(max++, x + px, z + pz, 2) == 0) r = bottomLeft;
                        if (bottomRight != 0 && randForPos(max++, x + px, z + pz, 3) == 0) r = bottomRight;

                        if (randForPos(3, x + px, z + pz, 4) == 0)
                            data[x + z * sx] = r;
                        else
                            data[x + z * sx] = 0;
                    }
                    //some other biome by the ocean
                    else if (center > 0 && (topLeft == 0 || topRight == 0 || bottomLeft == 0 || bottomRight == 0)) {
                        //if rand()%5 and id not icePlains
                        if (randForPos(5, x + px, z + pz, 5) == 0)
                                data[x + z * sx] = 0;
                        else
                            data[x + z * sx] = center;
                    }
                    //inland biome or full ocean
                    else
                        data[x + z * sx] = center;
                }
            }

            return data;
        }
    private:
        const BiomeFunction* base = nullptr;
};

#endif // BIOMEISLAND_HPP
