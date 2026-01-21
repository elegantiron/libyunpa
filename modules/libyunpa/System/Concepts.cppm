module;
#include <type_traits>
export module libyunpa.System:Concepts;

namespace libyunpa {
  export template <typename T>
  concept Arithmetic = std::is_integral_v<T> || std::is_floating_point_v<T>;
}
