module;
#include <atomic>
#include <cstdint>
#include <functional>
#include <queue>
#include <tao/pegtl.hpp>
#include <thread>
#include <variant>
#ifdef WIN32
#include <conio.h>
#endif
export module libyunpa:Events;
using namespace tao;

namespace libyunpa {
#pragma region Grammar

  namespace Grammar {
    struct ESC : pegtl::one<'\x1b'> {};

    struct CSI : pegtl::seq<ESC, pegtl::one<'['>> {};

    template <char delim = ';'>
    struct DigitPlusTerm
        : pegtl::seq<pegtl::plus<pegtl::digit>, pegtl::one<delim>> {};

    struct Win32InputString : pegtl::seq<CSI,
                                         DigitPlusTerm<>,
                                         DigitPlusTerm<>,
                                         DigitPlusTerm<>,
                                         DigitPlusTerm<>,
                                         DigitPlusTerm<>,
                                         DigitPlusTerm<'_'>> {};

    struct Language : pegtl::sor<Win32InputString> {};
  } // namespace Grammar

#pragma region Enums
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
#pragma region Events

  namespace Events {
    export struct KeyEvent {
      bool    isKeyDown;
      Keys    key;
      KeyMods mods;
    };
  } // namespace Events

  export using Event = std::variant<std::monostate, Events::KeyEvent>;

  using EventCallback = std::function<void(Event)>;

#pragma region Actions

  template <typename Rule>
  struct Action {
    template <typename ActionInput>
    static void apply(const ActionInput& /*actionInput*/,
                      const EventCallback& /*eventCallback*/) {}
  };

#pragma region EventMan

  class EventManager {
  private:
    std::atomic_flag _running = false;
    std::thread      _inputThread;

    std::mutex        _eventQueueMutex;
    std::queue<Event> _eventQueue;

    void enqueueEvent(Event event);
    void inputLoop();

  public:
    ~EventManager();
    EventManager(EventManager&)  = delete;
    EventManager(EventManager&&) = delete;

    EventManager& operator=(EventManager&)  = delete;
    EventManager& operator=(EventManager&&) = delete;

    void start();
    void stop();
    bool pollEvents(Event& event);
  };

  EventManager::~EventManager() {
    stop();
  }

  void EventManager::enqueueEvent(Event event) {
    std::lock_guard lock(_eventQueueMutex);
    _eventQueue.push(event);
  }

  void EventManager::stop() {
    if (_running.test()) {
    _running.clear();
    _running.notify_all();
    }
    if (_inputThread.joinable()) {
      _inputThread.join();
    }
  }

  void EventManager::start() {
    _running.test_and_set();
    _running.notify_all();
    _inputThread = std::thread(&EventManager::inputLoop, this);
  }

#ifdef WIN32
  void EventManager::inputLoop() {
    std::string workingString;
    while (_running.test()) {
      while (_kbhit() != 0) {
        auto input     = _getch();
        workingString += static_cast<char>(input);
      }
      if (workingString.empty()) {
        continue;
      }
      auto parserInput = pegtl::memory_input(workingString, "");
    }
  }
#endif
} // namespace libyunpa
