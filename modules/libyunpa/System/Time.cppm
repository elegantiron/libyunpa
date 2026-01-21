module;
#include <chrono>
export module libyunpa.System:Time;

namespace libyunpa {
  /// @brief A length of time
  export using Duration = std::chrono::duration<long double, std::milli>;
  /// @brief A point in time
  export using TimePoint
      = std::chrono::time_point<std::chrono::steady_clock, Duration>;

  constexpr auto ZERO_TIME = std::chrono::milliseconds(0);

  /// @brief Manages various clocks and timers for a game
  export class GameTime {
  private:
    Duration  _lastFrame        = ZERO_TIME;
    Duration  _totalElapsedTime = ZERO_TIME;
    TimePoint _frameStart       = std::chrono::steady_clock::now();

  public:
    /// @brief Reset the internal clocks
    void reset();
    /// @brief Update the internal clocks and timers
    void update();

    /// @brief Get the duration of the last frame
    [[nodiscard]] const Duration& getLastFrame() const;
    /// @brief Get the duration the game has been running
    [[nodiscard]] const Duration& getTotalElapsedTime() const;
  };

  void GameTime::update() {
    auto now           = std::chrono::steady_clock::now();
    _lastFrame         = now - _frameStart;
    _totalElapsedTime += _lastFrame;
    _frameStart        = now;
  }

  void GameTime::reset() {
    _lastFrame        = ZERO_TIME;
    _totalElapsedTime = ZERO_TIME;
    _frameStart       = std::chrono::steady_clock::now();
  }

  const Duration& GameTime::getLastFrame() const {
    return _lastFrame;
  }

  const Duration& GameTime::getTotalElapsedTime() const {
    return _totalElapsedTime;
  }
} // namespace libyunpa
