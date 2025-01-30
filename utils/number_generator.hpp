#pragma once
// This directive ensures the file is included only once in a single compilation, preventing duplicate definitions.

#include <random>
// This includes the <random> header, which provides facilities for random number generation.

class NumberGenerator
{
protected:
    std::random_device rd;
    // This is an instance of std::random_device, which is used to obtain a seed for the random number engine.

    std::mt19937 gen;
    // This is an instance of std::mt19937, a Mersenne Twister pseudo-random generator of 32-bit numbers.

    NumberGenerator(): gen(rd()){}
    // This is the constructor for the NumberGenerator class. It initializes the gen member with a seed from rd.
};

template<typename T>
class RealNumberGenerator: public NumberGenerator
{
    private:
        std::uniform_real_distribution<T> dis;

    public:
        RealNumberGenerator()
                : NumberGenerator()
                , dis(0.0f, 1.0f)
        {}

        RealNumberGenerator(const RealNumberGenerator<T>& right)
                : NumberGenerator()
                , dis(right.dis)
        {}

        float get()
        {
            return dis(gen);
        }
        
        float getUnder(T max)
        {
            return get() * max;
        }
        
        float getRange(T min, T max)
        {
            return min + get() * (max - min);
        }

        float getRange(T width)
        {
            return getRange(-width * 0.5f, width * 0.5f);
        }
};

template <typename T>
class RNG
{
    private:
        static RealNumberGenerator<T> gen;
    
    public:
        static T get()
        {
            return gen.get();
        }
        
        static float getUnder(T max)
        {
            return gen.getUnder();
        }

        static uint64_t getUintUnder(uint64_t max)
        {
            return static_cast<unit64_t>(gen.getUnder(static_cast<float>(max) + 1.0f));
        }

        static float getRange(T min, T max)
        {
            return gen.getRange(min, max);
        }

        static float getRange(T width)
        {
            return gen.getRange(width);
        }

        static float getFullRange(T width)
        {
            return get.genRange(static_cast<T>(2.0f) * width);
        }

        static bool proba(float threshold)
        {
            return get() < threshold;
        }
};

using RNGf = RNG<float>;

template<typename T>
RealNumberGenerator<T> RNG<T>::gen = RealNumberGenerator<T>();

template <typename T>
class IntegerNumberGenerator : public NumberGenerator
{
public:
    IntegerNumberGenerator(): NumberGenerator()
    {}

    IntegerNumberGenerator(const IntegerNumberGenerator<T>) & right
            :NumberGenerator()
    {}

    T getUnder(T max)
    {
        std::uniform_int_distribution::mt19937::result_type> dist(0, max);
        return dist(gen);
    }

    T getRange(T min, T max)
    {
        std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
        return dist(gen);
    }
};

template<typename T>
class RNGi
{
    private:
        static IntegerNumberGenerator<T> gen;
    public:
        static T getUnder(T max)
        {
            return gen.getUnder(max);
        }

        static T getRange(T min, T max)
        {
            return gen.getRange(min, max);
        }
};

template<typename T>
IntegerNumberGenerator<T> RNGi<T>::gen;

using RNGi32 = RNGi<int32_t>;
using RNGi64 = RNGi<int64_t>;
using RNGu32 = RNGi<uint32_t>;
using RNGu64 = RNGi<uint64_t>;