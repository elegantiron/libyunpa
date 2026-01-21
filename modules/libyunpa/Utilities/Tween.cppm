module;
#include <chrono>
#include <type_traits>

export module libyunpa.Utilities:Tween;
import libyunpa.System;

namespace libyunpa::Utils {
  const auto ZERO_TIME = std::chrono::milliseconds(0);

  /**
     @brief Options to use when constructing a Tweener

   */
  export template <Arithmetic T>
  struct TweenerOptions {
    /**
     *  @brief How long this Tweener should take to tween
     *
     */
    Duration length = ZERO_TIME;
    /**
     * @brief Whether this Tweener should crycle between each end.
     *
     */
    bool     cycles = false;
    /**
     * @brief The start point
     *
     */
    T        start;
    /**
     * @brief The end point
     *
     */
    T        end;
  };

  /**
     @brief Handles 'tweening integral and fractional types

   */
  export template <Arithmetic T>
  class Tweener {
    Duration _length  = ZERO_TIME;
    Duration _elapsed = ZERO_TIME;
    bool     _cycles  = false;
    T        _start;
    T        _end;

  public:
    /**
       @brief Update the Tweener

       @param elapsed
     */
    void update(const Duration& elapsed);
    /**
       @brief Get the 'tweened value

       @return An interpolated value
     */
    auto get() const -> T;
    /**
       @brief Construct a new Tweener object with the given options

       @param options
     */
    Tweener(TweenerOptions<T> options);
  };

  template <Arithmetic T>
  void Tweener<T>::update(const Duration& elapsed) {
    _elapsed += elapsed;
    if (_elapsed.count() > _length.count()) {
      _elapsed = _length;
    }
  }

  template <Arithmetic T>
  auto Tweener<T>::get() const -> T {
    if (_length.count() == 0) {
      return 0;
    }
    auto ratio = _elapsed.count() / _length.count();
    return static_cast<T>((_end - _start) * ratio);
  }
} // namespace libyunpa::Utils
