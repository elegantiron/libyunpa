module;
#include <cstdint>
export module libyunpa:Keyboard;

namespace libyunpa {
  export enum class Keys : std::uint8_t {
    MISSING,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z
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
