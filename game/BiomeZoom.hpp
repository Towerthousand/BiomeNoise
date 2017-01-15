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
        int choose(int par1, int par2, int x, int z, int n) const {
            return this->randForPos(2, x, z, n) == 0 ? par1 : par2;
        }

        int modeOrRandom(int par1, int par2, int par3, int par4, int x, int z, int n) const {
            if (par2 == par3 && par3 == par4) return par2;
            else if (par1 == par2 && par1 == par3) return par1;
            else if (par1 == par2 && par1 == par4) return par1;
            else if (par1 == par3 && par1 == par4) return par1;
            else if (par1 == par2 && par3 != par4) return par1;
            else if (par1 == par3 && par2 != par4) return par1;
            else if (par1 == par4 && par2 != par3) return par1;
            else if (par2 == par1 && par3 != par4) return par2;
            else if (par2 == par3 && par1 != par4) return par2;
            else if (par2 == par4 && par1 != par3) return par2;
            else if (par3 == par1 && par2 != par4) return par3;
            else if (par3 == par2 && par1 != par4) return par3;
            else if (par3 == par4 && par1 != par2) return par3;
            else if (par4 == par1 && par2 != par3) return par3;
            else if (par4 == par2 && par1 != par3) return par3;
            else if (par4 == par3 && par1 != par2) return par3;
            else
            {
                int var5 = this->randForPos(4, x, z, n);
                return var5 == 0 ? par1 : (var5 == 1 ? par2 : (var5 == 2 ? par3 : par4));
            }
        }

        const BiomeFunction* base = nullptr;
        const bool fuzzy = false;
};

#endif // BIOMEZOOM_HPP
