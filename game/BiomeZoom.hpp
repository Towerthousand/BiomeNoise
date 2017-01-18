#ifndef BIOMEZOOM_HPP
#define BIOMEZOOM_HPP
#include "BiomeFunction.hpp"

class BiomeZoom final : public BiomeFunction {
    public:
        BiomeZoom(std::mt19937* generator, BiomeFunction* base, bool fuzzy=false) : BiomeFunction(generator), base(base), fuzzy(fuzzy) {
            VBE_ASSERT_SIMPLE(base != nullptr);
        }

        virtual ~BiomeZoom() {
        }

        std::vector<int> getBiomeData(int px, int pz, int sx, int sz) const override {
            int baseX = px >> 1;
            int baseZ = pz >> 1;
            int baseSizeX = (sx >> 1) + 3;
            int baseSizeZ = (sz >> 1) + 3;
            int zoomedSizeX = baseSizeX * 2;
            int zoomedSizeZ = baseSizeZ * 2;
            std::vector<int>  baseData = base->getBiomeData(baseX, baseZ, baseSizeX, baseSizeZ);
            std::vector<int>  zoomedData = std::vector<int>(zoomedSizeX * zoomedSizeZ);

            for (int z = 0; z < baseSizeZ - 1; ++z)
            {
                int currZoomedZ = (z << 1) * zoomedSizeX;
                int topleft = baseData[0 + (z + 0) * baseSizeX]; //baseData[z][0]
                int bottomleft = baseData[0 + (z + 1) * baseSizeX]; //baseData[z+1][0]

                for (int x = 0; x < baseSizeX - 1; ++x)
                {
                    int topright = baseData[x + 1 + (z + 0) * baseSizeX]; //baseData[z][x+1]
                    int bottomright = baseData[x + 1 + (z + 1) * baseSizeX]; //baseData[z+1][x+1]
                    // Set topleft corner
                    zoomedData[currZoomedZ] = topleft;
                    // Set bottomleft corner
                    zoomedData[currZoomedZ++ + zoomedSizeX] = choose(topleft, bottomleft, x + baseX, z + baseZ, 0);
                    // Set topright corner
                    zoomedData[currZoomedZ] = choose(topleft, topright, x + baseX, z + baseZ, 1);
                    // Set bottomRight corner
                    if(fuzzy)
                        zoomedData[currZoomedZ++ + zoomedSizeX] = choose(topleft, topright, bottomleft, bottomright, x + baseX, z + baseZ, 2);
                    else
                        zoomedData[currZoomedZ++ + zoomedSizeX] = modeOrRandom(topleft, topright, bottomleft, bottomright, x + baseX, z + baseZ, 2);
                    topleft = topright;
                    bottomleft = bottomright;
                }
            }

            std::vector<int>  result = std::vector<int>(sx * sz);

            for (int z = 0; z < sz; ++z) {
                std::vector<int>::const_iterator start = zoomedData.begin() + (z + (pz & 1)) * zoomedSizeX + (px & 1);
                result.insert(
                    result.begin() + (z*sx),
                    start,
                    start+sx
                );
            }

            return result;
        }

    private:
        int choose(int a, int b, int x, int z, int n) const {
            return randForPos(2, x, z, n) == 0 ? a : b;
        }

        int choose(int a, int b, int c, int d, int x, int z, int n) const {
            int r = randForPos(4, x, z, n);
            return r == 0 ? a : (r == 1 ? b : (r == 2 ? c : d));
        }

        int modeOrRandom(int a, int b, int c, int d, int x, int z, int n) const {
            if (b == c && c == d) return b;
            else if (a == b && a == c) return a;
            else if (a == b && a == d) return a;
            else if (a == c && a == d) return a;
            else if (a == b && c != d) return a;
            else if (a == c && b != d) return a;
            else if (a == d && b != c) return a;
            else if (b == a && c != d) return b;
            else if (b == c && a != d) return b;
            else if (b == d && a != c) return b;
            else if (c == a && b != d) return c;
            else if (c == b && a != d) return c;
            else if (c == d && a != b) return c;
            else if (d == a && b != c) return c;
            else if (d == b && a != c) return c;
            else if (d == c && a != b) return c;
            else {
                int r = this->randForPos(4, x, z, n);
                return r == 0 ? a : (r == 1 ? b : (r == 2 ? c : d));
            }
        }

        const BiomeFunction* base = nullptr;
        const bool fuzzy = false;
};

#endif // BIOMEZOOM_HPP
