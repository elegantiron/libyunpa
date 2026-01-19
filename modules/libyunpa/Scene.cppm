module;
#include <ftxui/dom/node.hpp>
#include <memory>
#include <optional>
#include <stack>
#include <utility>

export module libyunpa:Scene;
import :Time;

namespace libyunpa::Engine {
  export class Scene;
  /// @brief A pointer to a Scene
  export using ScenePtr = std::shared_ptr<Scene>;

  /// @brief A discete part of a game
  export class Scene {
  private:
    ScenePtr _parent;
    bool     _wantsExit = false;

  protected:
    void requestExit();

  public:
    /// @brief Construct a scene with the given parent
    /// @param[in] parent This scene's parent
    Scene(ScenePtr parent);
    Scene();                  ///< @overload
    Scene(Scene&)  = default; ///< @overload
    Scene(Scene&&) = default; ///< @overload

    /// @brief Copy assignment operator
    Scene& operator=(const Scene&) = default;
    /// @brief Move assignment operator
    Scene& operator=(Scene&&) = default;

    virtual ~Scene() = default;

    [[nodiscard]] bool wantsExit() const;

    virtual void onBury();
    virtual void onReveal();

    /// @brief Update the scene
    /// @param[in] gameTime
    virtual void update(const GameTime& gameTime) = 0;

    /// @brief Render the Scene
    [[nodiscard]]
    virtual ftxui::Element render() const
        = 0;
  };

  /// @brief Indicate that this scene wants to exit
  void Scene::requestExit() {
    _wantsExit = true;
  }

  /// @brief Check whether this scene wants to exit
  bool Scene::wantsExit() const {
    return _wantsExit;
  }

  /// @brief Called before this scene is deactivated
  void Scene::onBury() {}

  /// @brief Called after this scene is reactivated
  void Scene::onReveal() {}

#pragma region Manager

  /// @brief Manages Scenes for a game
  class SceneManager {
  private:
    ScenePtr             _nextScene;
    std::stack<ScenePtr> _scenes;

    void transitionScene();
    void popScene();

  public:
    void setNextScene(ScenePtr nextScene);
    auto getCurrentScene() -> std::optional<ScenePtr>;
    void update(const GameTime& gameTime);
  };

  /// @brief Set the next scene to run
  /// @param[in] nextScene
  void SceneManager::setNextScene(ScenePtr nextScene) {
    _nextScene = std::move(nextScene);
  }

  /// @brief Get the currently active Scene
  auto SceneManager::getCurrentScene() -> std::optional<ScenePtr> {
    if (_scenes.empty()) {
      return {};
    }
    return _scenes.top();
  }

  /// @brief Update the currently active scene
  /// @param[in] gameTime
  void SceneManager::update(const GameTime& gameTime) {
    if (_scenes.empty() and _nextScene == nullptr) {
      return;
    }
    if (not _scenes.empty() and _scenes.top()->wantsExit()) {
      popScene();
    }
    if (_nextScene not_eq nullptr) {
      transitionScene();
    }
    if (_scenes.empty()) {
      return;
    }
    _scenes.top()->update(gameTime);
  }

  void SceneManager::transitionScene() {
    if (not _scenes.empty()) {
      _scenes.top()->onBury();
    }
    _scenes.push(_nextScene);
    _nextScene = nullptr;
  }

  void SceneManager::popScene() {
    if (_scenes.empty()) {
      return;
    }
    _scenes.pop();
    if (_scenes.empty()) {
      return;
    }
    _scenes.top()->onReveal();
  }
} // namespace libyunpa::Engine
