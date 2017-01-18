#ifndef BIOMEREPLACE_HPP
#define BIOMEREPLACE_HPP
#include "BiomeFunction.hpp"
#include "BiomeSet.hpp"

class BiomeReplace final : public BiomeFunction {
    public:
        BiomeReplace(std::mt19937* generator, BiomeFunction* base, BiomeSet from, std::vector<std::pair<int, int>> to, bool noEdges = false) :
            BiomeFunction(generator), base(base), from(from), to(to), noEdges(noEdges) {
            VBE_ASSERT_SIMPLE(base != nullptr);
            VBE_ASSERT_SIMPLE(!to.empty());
        }
        BiomeReplace(std::mt19937* generator, BiomeFunction* base, BiomeSet from, int to, int prob, bool noEdges = false) :
            BiomeFunction(generator), base(base), from(from), to({{to, 1}, {-1, prob-1}}), noEdges(noEdges) {
            VBE_ASSERT_SIMPLE(prob > 1);
            VBE_ASSERT_SIMPLE(base != nullptr);
        }

        virtual ~BiomeReplace() {
        }

        std::vector<int> getBiomeData(int px, int pz, int sx, int sz) const override {
            int s = 0;
            for(const std::pair<int, int>& t : to)
                s += t.second;

            if(noEdges) {
                std::vector<int> baseData = base->getBiomeData(px - 1, pz - 1, sx + 2, sz + 2);
                std::vector<int> result(sx * sz);

                for (int z = 0; z < sz; ++z) {
                    for (int x = 0; x < sx; ++x) {
                        int old = baseData[x + 1 + (z + 1) * (sx + 2)];
                        int val = old;
                        if(from.test(val)) {
                            int r = randForPos(s, px + x, pz + z, 0);
                            for(const std::pair<int, int>& p : to) {
                                r -= p.second;
                                if(r < 0) {
                                    if(p.first != -1)
                                        val = p.first;
                                    break;
                                }
                            }
                        }

                        if (val == old)
                            result[x + z * sx] = old;
                        else {
                            int top = baseData[x + 1 + (z + 1 - 1) * (sx + 2)];
                            int right = baseData[x + 1 + 1 + (z + 1) * (sx + 2)];
                            int left = baseData[x + 1 - 1 + (z + 1) * (sx + 2)];
                            int down = baseData[x + 1 + (z + 1 + 1) * (sx + 2)];

                            if (top == old && right == old && left == old && down == old)
                                result[x + z * sx] = val;
                            else
                                result[x + z * sx] = old;
                        }
                    }
                }
                return result;
            }

            std::vector<int> result = base->getBiomeData(px, pz, sx, sz);
            for (int z = 0; z < sz; ++z)
                for (int x = 0; x < sx; ++x)
                    if(from.test(result[x + z * sx])) {
                        int r = randForPos(s, px + x, pz + z, 0);
                        for(const std::pair<int, int>& p : to) {
                            r -= p.second;
                            if(r < 0) {
                                if(p.first != -1)
                                    result[x + z * sx] = p.first;
                                break;
                            }
                        }
                    }

            return result ;
        }

    private:
        const BiomeFunction* base = nullptr;
        const BiomeSet from;
        const std::vector<std::pair<int, int>> to;
        const bool noEdges = false;
};

#endif // BIOMEREPLACE_HPP
