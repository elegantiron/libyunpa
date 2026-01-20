module;
#include <cstdint>
export module libyunpa:Keyboard;

namespace libyunpa {
  /**
   * @brief Represents keys on a keyboard.
   */
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
    KEY_Z,
    KEY_ARROW_UP,
    KEY_ARROW_RIGHT,
    KEY_ARROW_DOWN,
    KEY_ARROW_LEFT,
    KP_0,
    KP_1,
    KP_2,
    KP_3,
    KP_4,
    KP_5,
    KP_6,
    KP_7,
    KP_8,
    KP_9
  };

  /**
   * @brief Represents modifier keys on a keyboard
   */
  // NOLINTNEXTLINE(readability-enum-initial-value)
  export enum class KeyMods : std::uint8_t {
    NONE,
    SHIFT,
    CTRL,
    ALT,
    CONTROL = CTRL
  };

} // namespace libyunpa
