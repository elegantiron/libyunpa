module;
#include <format>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <optional>
#include <utility>
#include <variant>

export module libyunpa.Engine:Core;
import :Events;
import :Scene;
import libyunpa.System;

namespace libyunpa::Engine {
  /// @brief Central class for controlling a game.
  export class Core {
  public:
    Core()       = delete;
    ~Core()      = delete;
    Core(Core&)  = delete;
    Core(Core&&) = delete;

    Core& operator=(Core&)  = delete;
    Core& operator=(Core&&) = delete;

  private:
    static GameTime      _gameTime;
    static SceneManager  _sceneMan;
    static EventManager  _eventMan;
    static ftxui::Screen _screen;

    static void GameLoop();
    static void Render(const ScenePtr& scene);

  public:
    static void SetNextScene(ScenePtr nextScene);
    static auto GetCurrentScene() -> std::optional<ScenePtr>;
    static void Run();
  };

  GameTime      Core::_gameTime;
  SceneManager  Core::_sceneMan;
  EventManager  Core::_eventMan;
  ftxui::Screen Core::_screen{ftxui::Screen::Create(ftxui::Dimension::Full())};

  /// @brief Set the next Scene to run
  /// @param[in] nextScene
  void Core::SetNextScene(ScenePtr nextScene) {
    _sceneMan.setNextScene(std::move(nextScene));
  }

  /// @brief Get the currently active Scene
  /// @returns An optional object containing the pointer to the current scene or
  /// not.
  auto Core::GetCurrentScene() -> std::optional<ScenePtr> {
    return _sceneMan.getCurrentScene();
  }

  void Core::GameLoop() {
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
      Render(_sceneMan.getCurrentScene().value_or(nullptr));
      std::cout << std::format("\x1b[1;1H{}", _screen.ToString());
    }
  }

  /// @brief Run the game
  void Core::Run() {
    _eventMan.start();
    _gameTime.reset();
    GameLoop();
    _eventMan.stop();
  }

  // NOLINTNEXTLINE(misc-no-recursion)
  void Core::Render(const ScenePtr& scene) {
    if (scene == nullptr) {
      return;
    }
    Render(scene->getParent());
    ftxui::Render(_screen, scene->render());
  }

} // namespace libyunpa::Engine
