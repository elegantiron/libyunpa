module;
#include <string>
#include <vector>

export module libyunpa.Utilities:Menu;
import :Helpers;
import libyunpa.System;
import ftxui;

namespace libyunpa::Utils {
  /// @brief Options to use when making a Menu
  export struct MenuOptions {
    /// @brief Title of the Menu
    std::string title;
    /// @brief @ref TextOptions "Options" to use when rendering the selected
    /// item
    TextOptions selectedOptions;
    /// @brief @ref TextOptions "Options" to use when rendering non-selected
    /// items
    TextOptions defaultOptions;
  };

  /// @brief A menu to use for user input
  export class Menu {
  private:
    std::string              _title;
    TextOptions              _selectedOptions;
    TextOptions              _defaultOptions;
    std::vector<std::string> _items;
    ftxui::Elements          _elements;
    int                      _index = 0;

    void updateElements();

  public:
    Menu(MenuOptions options);
    /** @overload */
    Menu()       = default;
    /** @overload */
    Menu(Menu&)  = default;
    /** @overload */
    Menu(Menu&&) = default;

    ~Menu() = default;

    /** @overload */
    Menu& operator=(const Menu&) = default;
    /** @overload */
    Menu& operator=(Menu&&)      = default;

    void addItem(std::string_view text);
    void handleKey(Keys key);
    void setSelectedOptions(TextOptions options);
    void setDefaultOptions(TextOptions options);

    [[nodiscard]] ftxui::Element render() const;
  };

  /// @brief Construct a new Menu
  /// @param[in] options
  Menu::Menu(MenuOptions options)
      : _title(std::move(options.title)),
        _selectedOptions(options.selectedOptions),
        _defaultOptions(options.defaultOptions) {}

  /// @brief Add an item to the menu
  /// @param[in] text
  void Menu::addItem(std::string_view text) {
    _items.emplace_back(text.data());
    updateElements();
  }

  /// @brief Handle a key press
  /// @param[in] key
  void Menu::handleKey(Keys key) {
    switch (key) {
      using enum Keys;
    case KEY_W:
    case KEY_ARROW_UP:
    case KP_8:
      if (--_index < 0) {
        _index = static_cast<int>(_items.size()) - 1;
      }
      break;
    case KEY_S:
    case KEY_ARROW_DOWN:
    case KP_2:
      if (++_index >= static_cast<int>(_items.size())) {
        _index = 0;
      }
      break;
    default: break;
    }
    updateElements();
  }

  /// @brief Set the style to use for the selected option
  /// @param[in] options
  void Menu::setSelectedOptions(TextOptions options) {
    _selectedOptions = options;
  }

  /// @brief Set the style to use for the non-selected options
  /// @param[in] options
  void Menu::setDefaultOptions(TextOptions options) {
    _defaultOptions = options;
  }

  /// @brief Render the scene
  /// @returns An ftxui::Element representing the menu
  ftxui::Element Menu::render() const {
    return ftxui::vbox(ftxui::hbox(
        ftxui::window(ftxui::text(_title), ftxui::vbox(_elements))));
  }

  void Menu::updateElements() {
    std::vector<ftxui::Element> elements;
    elements.reserve(_items.size());
    for (int i = 0; i < static_cast<int>(_items.size()); i++) {
      elements.push_back(
          RenderText(i == _index ? _selectedOptions : _defaultOptions,
                     _items[static_cast<size_t>(i)]));
    }
    _elements.swap(elements);
  }
} // namespace libyunpa::Utils
