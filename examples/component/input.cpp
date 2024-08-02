// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <memory>  // for allocator, __shared_ptr_access
#include <string>  // for char_traits, operator+, string, basic_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"  // for Ref


int main() {
  using namespace ftxui;

  // The data:
  std::string first_name;
  std::string last_name;
  std::string password;
  std::string phoneNumber;
  Region selection;
  std::string textToCopy;

  auto screen = ScreenInteractive::TerminalOutput();

  // The basic input components:
  Component input_first_name = Input(&first_name, "first name");
  Component input_last_name = Input(&last_name, "last name");

  // The password input component:
  InputOption password_option;
  password_option.password = true;
  Component input_password = Input(&password, "password", password_option);

  // The phone number input component:
  // We are using `CatchEvent` to filter out non-digit characters.
  Component input_phone_number = Input(&phoneNumber, "phone number");
  input_phone_number |= CatchEvent([&](Event event) {
    return event.is_character() && !std::isdigit(event.character()[0]);
  });
  input_phone_number |= CatchEvent([&](Event event) {
    return event.is_character() && phoneNumber.size() > 10;
  });

  // The component tree:
  auto component = Container::Vertical({
      input_first_name,
      input_last_name,
      input_password,
      input_phone_number,
  });

  // Tweak how the component tree is rendered:
  auto renderer = Renderer(component, [&] {
    return vbox({
               hbox(text(" First name : "), input_first_name->Render()),
               hbox(text(" Last name  : "), input_last_name->Render()),
               hbox(text(" Password   : "), input_password->Render()),
               hbox(text(" Phone num  : "), input_phone_number->Render()),
               separator(),
               text("Hello " + first_name + " " + last_name),
               text("Your password is " + password),
               text("Your phone number is " + phoneNumber),
               text("select_start " + std::to_string(selection.startx) + ";" + std::to_string(selection.starty)),
               text("select_end " + std::to_string(selection.endx) + ";" + std::to_string(selection.endy)),
               text("textToCopy " + textToCopy)
           }) |
           border | selectable([&textToCopy](std::string txtSelected){textToCopy = txtSelected;});
  });

  // TODO: Implement the double click on word to select the word
  // TODO: Implement the double click and drag to select word by word (optional)
  // TODO: Implement the tripple click to select an entire line 
  // TODO: Call onSelectionChange_ only when the selection indeed did change, not at every render?
  // TODO: Add a "selectable" flag in the pixel class and take it into account when selecting things

  renderer |= CatchEvent([&](Event event) { 

    return selectableCatchEvent(event);    
  });

  screen.Loop(renderer);
}
