#ifndef BIOMEFUNCITON_HPP
#define BIOMEFUNCITON_HPP
#include "commons.hpp"


class BiomeFunction { //abstract
    public:
        BiomeFunction(std::mt19937* generator) {
            randomOffset = (*generator)();
        }
        virtual ~BiomeFunction() {
        }

        virtual std::vector<int> getBiomeData(int px, int pz, int sx, int sz) const = 0;

    protected:
        int randomOffset = 0;
        const long multiplier = 0x5DEECE66Dl;
        const long addend = 0xBl;
        const long mask = (1l << 48) - 1;

        int randForPos(int max, int px, int pz, int n) const {
            long seed = px*341651197+pz*84719323+n*517375 + randomOffset;

            seed = (seed ^ multiplier) & mask;
            seed = (seed * multiplier + addend) & mask;
            if ((max & -max) == max) { // i.e., max is a power of 2
                seed = ((unsigned long) seed) >> (48-31);
                return (int)((max * seed) >> 31);
            }

            return (int)(((seed % max)+max)%max);
        }
};

#endif // BIOMEFUNCITON_HPP
