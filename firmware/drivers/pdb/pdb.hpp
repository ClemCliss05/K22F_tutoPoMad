#pragma once

#include <cstdint>

namespace Drivers {
/**
 * Configures the K22F PDB for precise periodic hardware triggering.
 */
class Pdb {
  public:
    enum class Prescaler : uint8_t {
        Div1 = 0U,
        Div2 = 1U,
        Div4 = 2U,
        Div8 = 3U,
        Div16 = 4U,
        Div32 = 5U,
        Div64 = 6U,
        Div128 = 7U
    };

    enum class Multiplier : uint8_t { X1 = 0U, X10 = 1U, X20 = 2U, X40 = 3U };

    Pdb(uint32_t busClockHz, Prescaler prescaler, Multiplier multiplier);

    void init();
    void stop();

    uint32_t getClockHz() const;

  protected:
    bool configure(uint64_t ticks);

    uint32_t busClockHz_;
    Prescaler prescaler_;
    Multiplier multiplier_;
};

/**
 * Configures the PDB trigger sequence used to periodically trigger the DAC.
 */
class PdbDac : public Pdb {
  public:
    PdbDac(uint32_t busClockHz, Prescaler prescaler, Multiplier multiplier);

    void init();
    bool start(uint32_t periodUs);
};

} // namespace Drivers