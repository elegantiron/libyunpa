module;
#include <functional>
#include <tao/pegtl.hpp>
#include <variant>

#ifdef WIN32
#include <conio.h>
#endif
export module libyunpa.System:Events;
import :Helpers;
import :Keyboard;
using namespace tao;

namespace libyunpa {

#pragma region Events

  namespace Events {
    /**
     * @brief Represents a key press or release event
     */
    export struct KeyEvent {
      /// @brief Whether this represents a key press or release
      bool    isKeyDown;
      /// @brief The pressed key
      Keys    key;
      /// @brief Any modifiers when the key was pressed
      KeyMods mods;
    };
  } // namespace Events

  /**
   * @brief An event for a game to process
   */
  export using Event = std::variant<std::monostate, Events::KeyEvent>;

  /**
   * @brief A callback function which accepts an Event
   *
   */
  export using EventCallback = std::function<void(Event)>;

} // namespace libyunpa
