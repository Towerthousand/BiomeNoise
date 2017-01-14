#ifndef NOISE_HPP
#define NOISE_HPP
#include "commons.hpp"

class Noise {
    public:
        Noise();
        float get(float x, float y) const;
        float octavedGet(float x, float y, unsigned int octaves) const;

        mutable float scale = 1.0f;
        float min = 0.0f;
        float max = 1.0f;

        void seed(unsigned long s);
    private:
        int fastfloor(const float x) const;
        float dot(const int* g, const float x, const float y) const;

        std::mt19937 generator;
        std::vector<int> perm;
        static const int grad3[12][3];
};

#endif // NOISE_HPP
