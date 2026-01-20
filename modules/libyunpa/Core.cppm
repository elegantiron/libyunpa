module;
#include <format>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <utility>
#include <variant>

export module libyunpa:Core;
import :Events;
import :Scene;
import :Time;

namespace libyunpa::Engine {
  /// @brief Central class for controlling a game.
  export class Core {
  private:
    static std::unique_ptr<Core> _instance;
    Core();

  public:
    static Core& getInstance();
    ~Core()      = default;
    Core(Core&)  = delete;
    Core(Core&&) = delete;

    Core& operator=(Core&)  = delete;
    Core& operator=(Core&&) = delete;

  private:
    GameTime      _gameTime;
    SceneManager  _sceneMan;
    EventManager  _eventMan;
    ftxui::Screen _screen;

    void gameLoop();
    void render(const ScenePtr& scene);

  public:
    void setNextScene(ScenePtr nextScene);
    auto getCurrentScene() -> std::optional<ScenePtr>;

    void run();
  };

  Core::Core() : _screen(ftxui::Screen::Create(ftxui::Dimension::Full())) {}

  std::unique_ptr<Core> Core::_instance = nullptr;

  /// @brief Get the currently active instance
  /// @returns A reference to the active instance
  /// @details An instance is created if one doesn't already exist.
  Core& Core::getInstance() {
    if (_instance == nullptr) {
      // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
      _instance.reset(new Core());
    }
    return *_instance;
  }

  /// @brief Set the next Scene to run
  /// @param[in] nextScene
  void Core::setNextScene(ScenePtr nextScene) {
    _sceneMan.setNextScene(std::move(nextScene));
  }

  /// @brief Get the currently active Scene
  /// @returns An optional object containing the pointer to the current scene or
  /// not.
  auto Core::getCurrentScene() -> std::optional<ScenePtr> {
    return _sceneMan.getCurrentScene();
  }

  void Core::gameLoop() {
    while (true) {
      _gameTime.update();
      _sceneMan.update(_gameTime);
      if (_sceneMan.empty()) {
        return;
      }
      while (auto event = _eventMan.pollEvents()) {
        _sceneMan.handleEvent(event.value_or(std::monostate{}));
      }
      _screen.Clear();
      render(_sceneMan.getCurrentScene().value_or(nullptr));
      std::cout << std::format("\x1b[1;1H{}", _screen.ToString());
    }
  }

  /// @brief Run the game
  void Core::run() {
    _eventMan.start();
    _gameTime.reset();
    gameLoop();
    _eventMan.stop();
  }

  // NOLINTNEXTLINE(misc-no-recursion)
  void Core::render(const ScenePtr& scene) {
    if (scene == nullptr) {
      return;
    }
    render(scene->getParent());
    ftxui::Render(_screen, scene->render());
  }
} // namespace libyunpa::Engine
