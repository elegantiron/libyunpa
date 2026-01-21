module;
#include <format>
#include <ftxui/dom/elements.hpp>
#include <iostream>
#include <string>
#include <utility>

#ifdef WIN32
#include <windows.h>
#endif
export module libyunpa.Utilities:Helpers;
import libyunpa.System;

namespace libyunpa {


  /// @brief Options to use when underlining text
  export enum class UnderlineStyle : std::uint8_t {
    /// @brief No underline
    NONE,
    /// @brief <u>Single underline</u>
    SINGLE,
    /// @brief Double underline
    DOUBLE
  };

  /// @brief Options to use when rendering text
  export struct TextOptions {
    /// @brief Color to use for the text
    ftxui::Color   textColor       = ftxui::Color::White;
    /// @brief Color to use for the background
    ftxui::Color   backgroundColor = ftxui::Color::Black;
    /// @brief Whether to @b bold the text
    bool           bold            = false;
    /// @brief Whether to @a italicize the text
    bool           italic          = false;
    /// @brief Which <u>underline</u> style to use
    UnderlineStyle underline       = UnderlineStyle::NONE;
    /// @brief Whether to <s>strike</s> the text
    bool           strike          = false;
    /// @brief Whether to invert the text colors
    bool           inverted        = false;
    /// @brief Whether to have the text blink
    bool           blink           = false;
  };

  ftxui::Element RenderText(const TextOptions& options, std::string_view text) {
    auto result = ftxui::text(text);
    result      = ftxui::color(options.textColor, result);
    result      = ftxui::bgcolor(options.backgroundColor, result);
    if (options.bold) {
      result = ftxui::bold(result);
    }
    if (options.italic) {
      result = ftxui::italic(result);
    }
    switch (options.underline) {
    case UnderlineStyle::SINGLE: result = ftxui::underlined(result); break;
    case UnderlineStyle::DOUBLE:
      result = ftxui::underlinedDouble(result);
      break;
    case UnderlineStyle::NONE: break;
    }
    if (options.strike) {
      result = ftxui::strikethrough(result);
    }
    if (options.inverted) {
      result = ftxui::inverted(result);
    }
    if (options.blink) {
      result = ftxui::blink(result);
    }
    return result;
  }
} // namespace libyunpa
