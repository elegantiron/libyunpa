module;
#include <atomic>
#include <mutex>
#include <optional>
#include <queue>
#include <tao/pegtl.hpp>
#include <thread>

#ifdef WIN32
#include <conio.h>
#include <windows.h>

#endif

export module libyunpa.Engine:Events;
import :Helpers;
import libyunpa.System;

namespace libyunpa::Engine {
  using namespace tao;

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

#pragma region Actions

  template <typename Rule>
  struct Action {
    template <typename ActionInput>
    static void apply(const ActionInput& /*actionInput*/,
                      const EventCallback& /*eventCallback*/) {}
  };

  template <>
  struct Action<Grammar::Win32InputString> {
    template <typename ActionInput>
    static void apply(const ActionInput&   actionInput,
                      const EventCallback& eventCallback) {
      std::string inputString = actionInput.string();

      inputString                          = inputString.substr(2);
      auto                  virtualKeyCode = ConvertAndTrim(inputString);
      [[maybe_unused]] auto scan           = ConvertAndTrim(inputString);
      [[maybe_unused]] auto unicodeValue   = ConvertAndTrim(inputString);
      auto                  isKeyDown      = ConvertAndTrim(inputString) == 1;
      [[maybe_unused]] auto controlKeys    = ConvertAndTrim(inputString);

      auto event = Events::KeyEvent{.isKeyDown = isKeyDown,
                                    .key       = ConvertMSVK(virtualKeyCode),
                                    .mods      = KeyMods::NONE};
      eventCallback(event);
    }
  };

  /// @brief Manages events for a game
  class EventManager {
  private:
    std::atomic_flag _running = false;
    std::thread      _inputThread;

    std::mutex        _eventQueueMutex;
    std::queue<Event> _eventQueue;

    void enqueueEvent(Event event);
    void inputLoop();
    void initializeTerminal();

  public:
    EventManager() = default;
    ~EventManager();
    EventManager(EventManager&)  = delete;
    EventManager(EventManager&&) = delete;

    EventManager& operator=(EventManager&)  = delete;
    EventManager& operator=(EventManager&&) = delete;

    void                 start();
    void                 stop();
    std::optional<Event> pollEvents();
  };

  EventManager::~EventManager() {
    stop();
  }

  void EventManager::enqueueEvent(Event event) {
    std::lock_guard lock(_eventQueueMutex);
    _eventQueue.push(event);
  }

  /// @brief Stop monitoring for events
  void EventManager::stop() {
    if (_running.test()) {
      _running.clear();
      _running.notify_all();
    }
    if (_inputThread.joinable()) {
      _inputThread.join();
    }
  }

  /// @brief Start monitoring for events
  void EventManager::start() {
    initializeTerminal();
    {
      using enum DecModes;
      DECSET(APPLICATION_CURSOR_KEYS);
      DECRST(SHOW_CURSOR);
      DECRST(AUTO_REPEAT_KEYS);
      DECSET(ANY_EVENT_TRACKING);
      DECSET(SGR_MOUSE_MODE);
      DECSET(ALTERNATE_BUFFER);
    }
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
      pegtl::parse<Grammar::Language, Action>(parserInput, [&](Event event) {
        enqueueEvent(event);
      });
      workingString.clear();
    }
  }
#endif
  /// @brief Poll for any waiting events
  /// @returns A std::optional object either containing an Event or not
  std::optional<Event> EventManager::pollEvents() {
    std::lock_guard lock(_eventQueueMutex);
    if (_eventQueue.empty()) {
      return {};
    }
    auto result = _eventQueue.front();
    _eventQueue.pop();
    return result;
  }

#ifdef WIN32
  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void EventManager::initializeTerminal() {
    auto* inputHandle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode        = (ENABLE_VIRTUAL_TERMINAL_INPUT
                  bitor ENABLE_MOUSE_INPUT
                  bitor ENABLE_WINDOW_INPUT
                  bitor ENABLE_EXTENDED_FLAGS
                  bitor ENABLE_PROCESSED_INPUT)
          bitand compl(ENABLE_ECHO_INPUT bitand ENABLE_QUICK_EDIT_MODE);
    SetConsoleMode(inputHandle, mode);

    auto* outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    mode               = (ENABLE_VIRTUAL_TERMINAL_PROCESSING
            bitor ENABLE_PROCESSED_OUTPUT
            bitor DISABLE_NEWLINE_AUTO_RETURN);
    SetConsoleMode(outputHandle, mode);
    using enum DecModes;
    DECSET(WIN32_INPUT_MODE);
  }
#endif
} // namespace libyunpa::Engine
