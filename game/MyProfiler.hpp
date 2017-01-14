#ifndef MYPROFILER_HPP
#define MYPROFILER_HPP
#include "commons.hpp"

class MyProfiler final : public Profiler {
    public:
        MyProfiler();
    private:
        void renderCustomInterface() const;
};

#endif // MYPROFILER_HPP
