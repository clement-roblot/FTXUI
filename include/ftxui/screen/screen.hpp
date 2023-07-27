#ifndef FTXUI_SCREEN_SCREEN_HPP
#define FTXUI_SCREEN_SCREEN_HPP

#include <cstdint>  // for uint8_t
#include <memory>
#include <string>  // for string, basic_string, allocator
#include <vector>  // for vector

#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/color.hpp"     // for Color, Color::Default
#include "ftxui/screen/terminal.hpp"  // for Dimensions

namespace ftxui {

typedef union
{
    struct
    {
      uint8_t blink : 1;
      uint8_t bold : 1;
      uint8_t dim : 1;
      uint8_t inverted : 1;
      uint8_t underlined : 1;
      uint8_t underlined_double : 1;
      uint8_t strikethrough : 1;
    };
    uint8_t all;
} Style;


/// @brief A unicode character and its associated style.
/// @ingroup screen
struct Pixel {
  bool operator==(const Pixel& other) const;
  bool isSameStyle(const Pixel& other) const;

  // The graphemes stored into the pixel. To support combining characters,
  // like: a⃦, this can potentially contain multiple codepoints.
  std::string character = " ";

  // The hyperlink associated with the pixel.
  // 0 is the default value, meaning no hyperlink.
  uint8_t hyperlink = 0;

  // Colors:
  Color background_color = Color::Default;
  Color foreground_color = Color::Default;

  // A bit field representing the style:
  Style style;
  
  bool automerge : 1;

  Pixel()
      : automerge(false),
        style{.all=0} { }
};

/// @brief Define how the Screen's dimensions should look like.
/// @ingroup screen
namespace Dimension {
Dimensions Fixed(int);
Dimensions Full();
}  // namespace Dimension

/// @brief A rectangular grid of Pixel.
/// @ingroup screen
class Screen {
 public:
  // Constructors:
  Screen(int dimx, int dimy);
  static Screen Create(Dimensions dimension);
  static Screen Create(Dimensions width, Dimensions height);

  // Access a character in the grid at a given position.
  std::string& at(int x, int y);
  const std::string& at(int x, int y) const;

  // Access a cell (Pixel) in the grid at a given position.
  Pixel& PixelAt(int x, int y);
  const Pixel& PixelAt(int x, int y) const;

  std::string ToString() const;

  // Print the Screen on to the terminal.
  void Print() const;

  // Get screen dimensions.
  int dimx() const { return dimx_; }
  int dimy() const { return dimy_; }

  // Move the terminal cursor n-lines up with n = dimy().
  std::string ResetPosition(bool clear = false) const;

  // Fill the screen with space.
  void Clear();

  void ApplyShader();

  struct Cursor {
    int x = 0;
    int y = 0;

    enum Shape {
      Hidden = 0,
      BlockBlinking = 1,
      Block = 2,
      UnderlineBlinking = 3,
      Underline = 4,
      BarBlinking = 5,
      Bar = 6,
    };
    Shape shape;
  };
  Cursor cursor() const { return cursor_; }
  void SetCursor(Cursor cursor) { cursor_ = cursor; }

  // Store an hyperlink in the screen. Return the id of the hyperlink. The id is
  // used to identify the hyperlink when the user click on it.
  uint8_t RegisterHyperlink(const std::string& link);
  const std::string& Hyperlink(uint8_t id) const;

  Box stencil;

 protected:
  int dimx_;
  int dimy_;
  std::vector<std::vector<Pixel>> pixels_;
  Cursor cursor_;
  std::vector<std::string> hyperlinks_ = {""};
};

}  // namespace ftxui

#endif  // FTXUI_SCREEN_SCREEN_HPP

// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
