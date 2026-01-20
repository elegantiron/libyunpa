module;
#include <format>
#include <ftxui/dom/elements.hpp>
#include <iostream>
#include <string>
#include <utility>

#ifdef WIN32
#include <windows.h>
#endif
export module libyunpa:Helpers;
import :Keyboard;

namespace libyunpa {
  int ConvertAndTrim(std::string& input, char delim = ';') {
    auto count  = input.find(delim);
    auto result = std::stoi(input.substr(0, count));
    input       = input.substr(count + 1);
    return result;
  }

#ifdef WIN32
  Keys ConvertMSVK(int msvk) {
    switch (msvk) {
    case 'A': return Keys::KEY_A;
    case 'B': return Keys::KEY_B;
    case 'C': return Keys::KEY_C;
    case 'D': return Keys::KEY_D;
    case 'E': return Keys::KEY_E;
    case 'F': return Keys::KEY_F;
    case 'G': return Keys::KEY_G;
    case 'H': return Keys::KEY_H;
    case 'I': return Keys::KEY_I;
    case 'J': return Keys::KEY_J;
    case 'K': return Keys::KEY_K;
    case 'L': return Keys::KEY_L;
    case 'M': return Keys::KEY_M;
    case 'N': return Keys::KEY_N;
    case 'O': return Keys::KEY_O;
    case 'P': return Keys::KEY_P;
    case 'Q': return Keys::KEY_Q;
    case 'R': return Keys::KEY_R;
    case 'S': return Keys::KEY_S;
    case 'T': return Keys::KEY_T;
    case 'U': return Keys::KEY_U;
    case 'V': return Keys::KEY_V;
    case 'W': return Keys::KEY_W;
    case 'X': return Keys::KEY_X;
    case 'Y': return Keys::KEY_Y;
    case 'Z': return Keys::KEY_Z;
    default: return Keys::MISSING;
    }
  }
#endif
  enum class DecModes : std::uint16_t {
    APPLICATION_CURSOR_KEYS = 1,
    AUTO_REPEAT_KEYS        = 8,
    SHOW_CURSOR             = 25,
    APPLICATION_KEYPAD_MODE = 66,
    ANY_EVENT_TRACKING      = 1003,
    SEND_FOCUS_EVENTS       = 1004,
    SGR_MOUSE_MODE          = 1006,
    ALTERNATE_BUFFER        = 1049,
    WIN32_INPUT_MODE        = 9001
  };

  void DECSET(DecModes mode) {
    std::cout << std::format("\x1b[?{}h", std::to_underlying(mode));
  }

  void DECRST(DecModes mode) {
    std::cout << std::format("\x1b[?{}l", std::to_underlying(mode));
  }
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
    ftxui::Color textColor = ftxui::Color::White;
    /// @brief Color to use for the background
    ftxui::Color backgroundColor = ftxui::Color::Black;
    /// @brief Whether to @b bold the text
    bool bold = false;
    /// @brief Whether to @a italicize the text
    bool italic = false;
    /// @brief Which <u>underline</u> style to use
    UnderlineStyle underline = UnderlineStyle::NONE;
    /// @brief Whether to <s>strike</s> the text
    bool strike = false;
    /// @brief Whether to invert the text colors
    bool inverted = false;
    /// @brief Whether to have the text blink
    bool blink = false;
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
