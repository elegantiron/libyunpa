module;
#include <cstdint>
export module libyunpa:Keyboard;

namespace libyunpa {
  export enum class Keys {
  };

  // NOLINTNEXTLINE(readability-enum-initial-value)
  export enum class KeyMods : std::uint8_t {
    NONE,
    SHIFT,
    CTRL,
    ALT,
    CONTROL = CTRL
  };

} // namespace libyunpa
