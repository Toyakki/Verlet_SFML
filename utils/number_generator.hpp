#pragma once

#include <random>

// Base class for number generators
class NumberGenerator {
 protected:
    std::random_device rd_;  // Seed generator
    std::mt19937 gen_;       // Mersenne Twister pseudo-random generator

    NumberGenerator() : gen_(rd_()) {}  // Initialize generator with random seed

    virtual ~NumberGenerator() = default;
};

// Template class for generating real numbers (floating-point)
template <typename T>
class RealNumberGenerator : public NumberGenerator {
 private:
    std::uniform_real_distribution<T> distribution_;

 public:
    RealNumberGenerator()
        : NumberGenerator(), distribution_(0.0f, 1.0f) {}

    explicit RealNumberGenerator(const RealNumberGenerator<T>& right)
        : NumberGenerator(), distribution_(right.distribution_) {}

    T Get() { return distribution_(gen_); }

    T GetUnder(T max) { return Get() * max; }

    T GetRange(T min, T max) { return min + Get() * (max - min); }

    T GetRange(T width) { return GetRange(-width * 0.5f, width * 0.5f); }
};

// Static random number generator for real numbers
template <typename T>
class RNG {
 private:
    static RealNumberGenerator<T> generator_;

 public:
    static T Get() { return generator_.Get(); }

    static T GetUnder(T max) { return generator_.GetUnder(max); }

    static uint64_t GetUintUnder(uint64_t max) {
        return static_cast<uint64_t>(generator_.GetUnder(static_cast<T>(max) + 1.0f));
    }

    static T GetRange(T min, T max) { return generator_.GetRange(min, max); }

    static T GetRange(T width) { return generator_.GetRange(width); }

    static T GetFullRange(T width) { return generator_.GetRange(static_cast<T>(2.0f) * width); }

    static bool Proba(float threshold) { return Get() < threshold; }
};

// Define float-based random number generator
using RNGf = RNG<float>;

// Template specialization for real number generator
template <typename T>
RealNumberGenerator<T> RNG<T>::generator_ = RealNumberGenerator<T>();

// Template class for generating integer numbers
template <typename T>
class IntegerNumberGenerator : public NumberGenerator {
 public:
    IntegerNumberGenerator() : NumberGenerator() {}

    explicit IntegerNumberGenerator(const IntegerNumberGenerator<T>& right)
        : NumberGenerator() {}

    T GetUnder(T max) {
        std::uniform_int_distribution<T> dist(0, max);
        return dist(gen_);
    }

    T GetRange(T min, T max) {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(gen_);
    }
};

// Static random number generator for integers
template <typename T>
class RNGi {
 private:
    static IntegerNumberGenerator<T> generator_;

 public:
    static T GetUnder(T max) { return generator_.GetUnder(max); }

    static T GetRange(T min, T max) { return generator_.GetRange(min, max); }
};

// Template specialization for integer number generator
template <typename T>
IntegerNumberGenerator<T> RNGi<T>::generator_;

using RNGi32 = RNGi<int32_t>;
using RNGi64 = RNGi<int64_t>;
using RNGu32 = RNGi<uint32_t>;
using RNGu64 = RNGi<uint64_t>;
