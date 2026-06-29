class SplitMix64 {
public:
  explicit SplitMix64(uint64_t seed) : state_(seed) {}

  uint64_t operator()() {
    uint64_t z = (state_ += 0x9E3779B97F4A7C15ULL);

    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
  }

private:
  uint64_t state_;
};