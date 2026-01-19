module;
#include <tao/pegtl.hpp>
export module libyunpa:Parsing;
using namespace tao;

namespace libyunpa::Grammar {
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
} // namespace libyunpa::Grammar
