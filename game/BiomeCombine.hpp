#ifndef BIOMECOMBINE_HPP
#define BIOMECOMBINE_HPP
#include "BiomeFunction.hpp"

class BiomeCombine final : public BiomeFunction {
    public:
        BiomeCombine(std::mt19937* generator, BiomeFunction* baseA, BiomeFunction* baseB, BiomeSet setA, BiomeSet setB, int replace=-1)
            : BiomeFunction(generator), baseA(baseA), baseB(baseB), setA(setA), setB(setB), replace(replace) {
        }
        virtual ~BiomeCombine() {
        }

        std::vector<int> getBiomeData(int px, int pz, int sx, int sz) const override {
            std::vector<int> dataA = baseA->getBiomeData(px, pz, sx, sz);
            std::vector<int> dataB = baseB->getBiomeData(px, pz, sx, sz);

            for(int i = 0; i < sx*sz; i++) {
                if(setA.test(dataA[i]) && setB.test(dataB[i])) {
                    if(replace == -1) dataA[i] = dataB[i];
                    else dataA[i] = replace;
                }
            }

            return dataA;
        }
    private:
        const BiomeFunction* baseA = nullptr;
        const BiomeFunction* baseB = nullptr;
        const BiomeSet setA;
        const BiomeSet setB;
        const int replace = -1;
};

#endif // BIOMECOMBINE_HPP
