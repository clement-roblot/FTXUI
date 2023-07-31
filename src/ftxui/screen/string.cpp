// Content of this file was created thanks to:
// -
// https://www.unicode.org/Public/UCD/latest/ucd/auxiliary/WordBreakProperty.txt
// - Markus Kuhn -- 2007-05-26 (Unicode 5.0)
//   http://www.cl.cam.ac.uk/~mgk25/ucs/wcwidth.c
// Thanks you!

#include "ftxui/screen/string.hpp"

#include <array>    // for array
#include <cstddef>  // for size_t
#include <cstdint>  // for uint32_t, uint8_t, uint16_t, int32_t
#include <string>   // for string, basic_string, wstring
#include <tuple>    // for _Swallow_assign, ignore

#include "ftxui/screen/deprecated.hpp"       // for wchar_width, wstring_width
#include "ftxui/screen/string_internal.hpp"  // for WordBreakProperty, EatCodePoint, CodepointToWordBreakProperty, GlyphCount, GlyphIterate, GlyphNext, GlyphPrevious, IsCombining, IsControl, IsFullWidth, Utf8ToWordBreakProperty

namespace {

struct Interval {
  uint32_t first;
  uint32_t last;
};

static const std::array<Interval, 294> g_extend_characters = {{
    {0x300, 0x36f},     {0x483, 0x489},     {0x591, 0x5bd},
    {0x5bf, 0x5bf},     {0x5c1, 0x5c2},     {0x5c4, 0x5c5},
    {0x5c7, 0x5c7},     {0x610, 0x61a},     {0x64b, 0x65f},
    {0x670, 0x670},     {0x6d6, 0x6dc},     {0x6df, 0x6e4},
    {0x6e7, 0x6e8},     {0x6ea, 0x6ed},     {0x711, 0x711},
    {0x730, 0x74a},     {0x7a6, 0x7b0},     {0x7eb, 0x7f3},
    {0x7fd, 0x7fd},     {0x816, 0x819},     {0x81b, 0x823},
    {0x825, 0x827},     {0x829, 0x82d},     {0x859, 0x85b},
    {0x8d3, 0x8e1},     {0x8e3, 0x903},     {0x93a, 0x93c},
    {0x93e, 0x94f},     {0x951, 0x957},     {0x962, 0x963},
    {0x981, 0x983},     {0x9bc, 0x9bc},     {0x9be, 0x9c4},
    {0x9c7, 0x9c8},     {0x9cb, 0x9cd},     {0x9d7, 0x9d7},
    {0x9e2, 0x9e3},     {0x9fe, 0x9fe},     {0xa01, 0xa03},
    {0xa3c, 0xa3c},     {0xa3e, 0xa42},     {0xa47, 0xa48},
    {0xa4b, 0xa4d},     {0xa51, 0xa51},     {0xa70, 0xa71},
    {0xa75, 0xa75},     {0xa81, 0xa83},     {0xabc, 0xabc},
    {0xabe, 0xac5},     {0xac7, 0xac9},     {0xacb, 0xacd},
    {0xae2, 0xae3},     {0xafa, 0xaff},     {0xb01, 0xb03},
    {0xb3c, 0xb3c},     {0xb3e, 0xb44},     {0xb47, 0xb48},
    {0xb4b, 0xb4d},     {0xb55, 0xb57},     {0xb62, 0xb63},
    {0xb82, 0xb82},     {0xbbe, 0xbc2},     {0xbc6, 0xbc8},
    {0xbca, 0xbcd},     {0xbd7, 0xbd7},     {0xc00, 0xc04},
    {0xc3e, 0xc44},     {0xc46, 0xc48},     {0xc4a, 0xc4d},
    {0xc55, 0xc56},     {0xc62, 0xc63},     {0xc81, 0xc83},
    {0xcbc, 0xcbc},     {0xcbe, 0xcc4},     {0xcc6, 0xcc8},
    {0xcca, 0xccd},     {0xcd5, 0xcd6},     {0xce2, 0xce3},
    {0xd00, 0xd03},     {0xd3b, 0xd3c},     {0xd3e, 0xd44},
    {0xd46, 0xd48},     {0xd4a, 0xd4d},     {0xd57, 0xd57},
    {0xd62, 0xd63},     {0xd81, 0xd83},     {0xdca, 0xdca},
    {0xdcf, 0xdd4},     {0xdd6, 0xdd6},     {0xdd8, 0xddf},
    {0xdf2, 0xdf3},     {0xe31, 0xe31},     {0xe34, 0xe3a},
    {0xe47, 0xe4e},     {0xeb1, 0xeb1},     {0xeb4, 0xebc},
    {0xec8, 0xecd},     {0xf18, 0xf19},     {0xf35, 0xf35},
    {0xf37, 0xf37},     {0xf39, 0xf39},     {0xf3e, 0xf3f},
    {0xf71, 0xf84},     {0xf86, 0xf87},     {0xf8d, 0xf97},
    {0xf99, 0xfbc},     {0xfc6, 0xfc6},     {0x102b, 0x103e},
    {0x1056, 0x1059},   {0x105e, 0x1060},   {0x1062, 0x1064},
    {0x1067, 0x106d},   {0x1071, 0x1074},   {0x1082, 0x108d},
    {0x108f, 0x108f},   {0x109a, 0x109d},   {0x135d, 0x135f},
    {0x1712, 0x1714},   {0x1732, 0x1734},   {0x1752, 0x1753},
    {0x1772, 0x1773},   {0x17b4, 0x17d3},   {0x17dd, 0x17dd},
    {0x180b, 0x180d},   {0x1885, 0x1886},   {0x18a9, 0x18a9},
    {0x1920, 0x192b},   {0x1930, 0x193b},   {0x1a17, 0x1a1b},
    {0x1a55, 0x1a5e},   {0x1a60, 0x1a7c},   {0x1a7f, 0x1a7f},
    {0x1ab0, 0x1ac0},   {0x1b00, 0x1b04},   {0x1b34, 0x1b44},
    {0x1b6b, 0x1b73},   {0x1b80, 0x1b82},   {0x1ba1, 0x1bad},
    {0x1be6, 0x1bf3},   {0x1c24, 0x1c37},   {0x1cd0, 0x1cd2},
    {0x1cd4, 0x1ce8},   {0x1ced, 0x1ced},   {0x1cf4, 0x1cf4},
    {0x1cf7, 0x1cf9},   {0x1dc0, 0x1df9},   {0x1dfb, 0x1dff},
    {0x200c, 0x200c},   {0x20d0, 0x20f0},   {0x2cef, 0x2cf1},
    {0x2d7f, 0x2d7f},   {0x2de0, 0x2dff},   {0x302a, 0x302f},
    {0x3099, 0x309a},   {0xa66f, 0xa672},   {0xa674, 0xa67d},
    {0xa69e, 0xa69f},   {0xa6f0, 0xa6f1},   {0xa802, 0xa802},
    {0xa806, 0xa806},   {0xa80b, 0xa80b},   {0xa823, 0xa827},
    {0xa82c, 0xa82c},   {0xa880, 0xa881},   {0xa8b4, 0xa8c5},
    {0xa8e0, 0xa8f1},   {0xa8ff, 0xa8ff},   {0xa926, 0xa92d},
    {0xa947, 0xa953},   {0xa980, 0xa983},   {0xa9b3, 0xa9c0},
    {0xa9e5, 0xa9e5},   {0xaa29, 0xaa36},   {0xaa43, 0xaa43},
    {0xaa4c, 0xaa4d},   {0xaa7b, 0xaa7d},   {0xaab0, 0xaab0},
    {0xaab2, 0xaab4},   {0xaab7, 0xaab8},   {0xaabe, 0xaabf},
    {0xaac1, 0xaac1},   {0xaaeb, 0xaaef},   {0xaaf5, 0xaaf6},
    {0xabe3, 0xabea},   {0xabec, 0xabed},   {0xfb1e, 0xfb1e},
    {0xfe00, 0xfe0f},   {0xfe20, 0xfe2f},   {0xff9e, 0xff9f},
    {0x101fd, 0x101fd}, {0x102e0, 0x102e0}, {0x10376, 0x1037a},
    {0x10a01, 0x10a03}, {0x10a05, 0x10a06}, {0x10a0c, 0x10a0f},
    {0x10a38, 0x10a3a}, {0x10a3f, 0x10a3f}, {0x10ae5, 0x10ae6},
    {0x10d24, 0x10d27}, {0x10eab, 0x10eac}, {0x10f46, 0x10f50},
    {0x11000, 0x11002}, {0x11038, 0x11046}, {0x1107f, 0x11082},
    {0x110b0, 0x110ba}, {0x11100, 0x11102}, {0x11127, 0x11134},
    {0x11145, 0x11146}, {0x11173, 0x11173}, {0x11180, 0x11182},
    {0x111b3, 0x111c0}, {0x111c9, 0x111cc}, {0x111ce, 0x111cf},
    {0x1122c, 0x11237}, {0x1123e, 0x1123e}, {0x112df, 0x112ea},
    {0x11300, 0x11303}, {0x1133b, 0x1133c}, {0x1133e, 0x11344},
    {0x11347, 0x11348}, {0x1134b, 0x1134d}, {0x11357, 0x11357},
    {0x11362, 0x11363}, {0x11366, 0x1136c}, {0x11370, 0x11374},
    {0x11435, 0x11446}, {0x1145e, 0x1145e}, {0x114b0, 0x114c3},
    {0x115af, 0x115b5}, {0x115b8, 0x115c0}, {0x115dc, 0x115dd},
    {0x11630, 0x11640}, {0x116ab, 0x116b7}, {0x1171d, 0x1172b},
    {0x1182c, 0x1183a}, {0x11930, 0x11935}, {0x11937, 0x11938},
    {0x1193b, 0x1193e}, {0x11940, 0x11940}, {0x11942, 0x11943},
    {0x119d1, 0x119d7}, {0x119da, 0x119e0}, {0x119e4, 0x119e4},
    {0x11a01, 0x11a0a}, {0x11a33, 0x11a39}, {0x11a3b, 0x11a3e},
    {0x11a47, 0x11a47}, {0x11a51, 0x11a5b}, {0x11a8a, 0x11a99},
    {0x11c2f, 0x11c36}, {0x11c38, 0x11c3f}, {0x11c92, 0x11ca7},
    {0x11ca9, 0x11cb6}, {0x11d31, 0x11d36}, {0x11d3a, 0x11d3a},
    {0x11d3c, 0x11d3d}, {0x11d3f, 0x11d45}, {0x11d47, 0x11d47},
    {0x11d8a, 0x11d8e}, {0x11d90, 0x11d91}, {0x11d93, 0x11d97},
    {0x11ef3, 0x11ef6}, {0x16af0, 0x16af4}, {0x16b30, 0x16b36},
    {0x16f4f, 0x16f4f}, {0x16f51, 0x16f87}, {0x16f8f, 0x16f92},
    {0x16fe4, 0x16fe4}, {0x16ff0, 0x16ff1}, {0x1bc9d, 0x1bc9e},
    {0x1d165, 0x1d169}, {0x1d16d, 0x1d172}, {0x1d17b, 0x1d182},
    {0x1d185, 0x1d18b}, {0x1d1aa, 0x1d1ad}, {0x1d242, 0x1d244},
    {0x1da00, 0x1da36}, {0x1da3b, 0x1da6c}, {0x1da75, 0x1da75},
    {0x1da84, 0x1da84}, {0x1da9b, 0x1da9f}, {0x1daa1, 0x1daaf},
    {0x1e000, 0x1e006}, {0x1e008, 0x1e018}, {0x1e01b, 0x1e021},
    {0x1e023, 0x1e024}, {0x1e026, 0x1e02a}, {0x1e130, 0x1e136},
    {0x1e2ec, 0x1e2ef}, {0x1e8d0, 0x1e8d6}, {0x1e944, 0x1e94a},
    {0x1f3fb, 0x1f3ff}, {0xe0020, 0xe007f}, {0xe0100, 0xe01ef},
}};

// As of Unicode 13.0.0
static const std::array<Interval, 116> g_full_width_characters = {{
    {0x01100, 0x0115f}, {0x0231a, 0x0231b}, {0x02329, 0x0232a},
    {0x023e9, 0x023ec}, {0x023f0, 0x023f0}, {0x023f3, 0x023f3},
    {0x025fd, 0x025fe}, {0x02614, 0x02615}, {0x02648, 0x02653},
    {0x0267f, 0x0267f}, {0x02693, 0x02693}, {0x026a1, 0x026a1},
    {0x026aa, 0x026ab}, {0x026bd, 0x026be}, {0x026c4, 0x026c5},
    {0x026ce, 0x026ce}, {0x026d4, 0x026d4}, {0x026ea, 0x026ea},
    {0x026f2, 0x026f3}, {0x026f5, 0x026f5}, {0x026fa, 0x026fa},
    {0x026fd, 0x026fd}, {0x02705, 0x02705}, {0x0270a, 0x0270b},
    {0x02728, 0x02728}, {0x0274c, 0x0274c}, {0x0274e, 0x0274e},
    {0x02753, 0x02755}, {0x02757, 0x02757}, {0x02795, 0x02797},
    {0x027b0, 0x027b0}, {0x027bf, 0x027bf}, {0x02b1b, 0x02b1c},
    {0x02b50, 0x02b50}, {0x02b55, 0x02b55}, {0x02e80, 0x02e99},
    {0x02e9b, 0x02ef3}, {0x02f00, 0x02fd5}, {0x02ff0, 0x02ffb},
    {0x03000, 0x0303e}, {0x03041, 0x03096}, {0x03099, 0x030ff},
    {0x03105, 0x0312f}, {0x03131, 0x0318e}, {0x03190, 0x031e3},
    {0x031f0, 0x0321e}, {0x03220, 0x03247}, {0x03250, 0x04dbf},
    {0x04e00, 0x0a48c}, {0x0a490, 0x0a4c6}, {0x0a960, 0x0a97c},
    {0x0ac00, 0x0d7a3}, {0x0f900, 0x0faff}, {0x0fe10, 0x0fe19},
    {0x0fe30, 0x0fe52}, {0x0fe54, 0x0fe66}, {0x0fe68, 0x0fe6b},
    {0x0ff01, 0x0ff60}, {0x0ffe0, 0x0ffe6}, {0x16fe0, 0x16fe4},
    {0x16ff0, 0x16ff1}, {0x17000, 0x187f7}, {0x18800, 0x18cd5},
    {0x18d00, 0x18d08}, {0x1b000, 0x1b11e}, {0x1b150, 0x1b152},
    {0x1b164, 0x1b167}, {0x1b170, 0x1b2fb}, {0x1f004, 0x1f004},
    {0x1f0cf, 0x1f0cf}, {0x1f18e, 0x1f18e}, {0x1f191, 0x1f19a},
    {0x1f200, 0x1f202}, {0x1f210, 0x1f23b}, {0x1f240, 0x1f248},
    {0x1f250, 0x1f251}, {0x1f260, 0x1f265}, {0x1f300, 0x1f320},
    {0x1f32d, 0x1f335}, {0x1f337, 0x1f37c}, {0x1f37e, 0x1f393},
    {0x1f3a0, 0x1f3ca}, {0x1f3cf, 0x1f3d3}, {0x1f3e0, 0x1f3f0},
    {0x1f3f4, 0x1f3f4}, {0x1f3f8, 0x1f43e}, {0x1f440, 0x1f440},
    {0x1f442, 0x1f4fc}, {0x1f4ff, 0x1f53d}, {0x1f54b, 0x1f54e},
    {0x1f550, 0x1f567}, {0x1f57a, 0x1f57a}, {0x1f595, 0x1f596},
    {0x1f5a4, 0x1f5a4}, {0x1f5fb, 0x1f64f}, {0x1f680, 0x1f6c5},
    {0x1f6cc, 0x1f6cc}, {0x1f6d0, 0x1f6d2}, {0x1f6d5, 0x1f6d7},
    {0x1f6eb, 0x1f6ec}, {0x1f6f4, 0x1f6fc}, {0x1f7e0, 0x1f7eb},
    {0x1f90c, 0x1f93a}, {0x1f93c, 0x1f945}, {0x1f947, 0x1f978},
    {0x1f97a, 0x1f9cb}, {0x1f9cd, 0x1f9ff}, {0x1fa70, 0x1fa74},
    {0x1fa78, 0x1fa7a}, {0x1fa80, 0x1fa86}, {0x1fa90, 0x1faa8},
    {0x1fab0, 0x1fab6}, {0x1fac0, 0x1fac2}, {0x1fad0, 0x1fad6},
    {0x20000, 0x2fffd}, {0x30000, 0x3fffd},
}};

using WBP = ftxui::WordBreakProperty;
struct WordBreakPropertyInterval {
  uint32_t first;
  uint32_t last;
  WBP property;
};

// Properties from:
// https://www.unicode.org/Public/UCD/latest/ucd/auxiliary/WordBreakProperty.txt
static const std::array<WordBreakPropertyInterval, 648> g_word_break_intervals = {{
  {0x00a, 0x00a, WBP::LF},
  {0x00b, 0x00c, WBP::Newline},
  {0x00d, 0x00d, WBP::CR},
  {0x020, 0x020, WBP::WSegSpace},
  {0x022, 0x022, WBP::Double_Quote},
  {0x027, 0x027, WBP::Single_Quote},
  {0x02c, 0x02c, WBP::MidNum},
  {0x02e, 0x02e, WBP::MidNumLet},
  {0x030, 0x039, WBP::Numeric},
  {0x03a, 0x03a, WBP::MidLetter},
  {0x03b, 0x03b, WBP::MidNum},
  {0x03c, 0x05e, WBP::ALetter},
  {0x05f, 0x05f, WBP::ExtendNumLet},
  {0x060, 0x084, WBP::ALetter},
  {0x085, 0x085, WBP::Newline},
  {0x086, 0x0ac, WBP::ALetter},
  {0x0ad, 0x0ad, WBP::Format},
  {0x0ae, 0x0b6, WBP::ALetter},
  {0x0b7, 0x0b7, WBP::MidLetter},
  {0x0b8, 0x2ff, WBP::ALetter},
  {0x300, 0x36f, WBP::Extend},
  {0x370, 0x37d, WBP::ALetter},
  {0x37e, 0x37e, WBP::MidNum},
  {0x37f, 0x386, WBP::ALetter},
  {0x387, 0x387, WBP::MidLetter},
  {0x388, 0x482, WBP::ALetter},
  {0x483, 0x489, WBP::Extend},
  {0x48a, 0x55e, WBP::ALetter},
  {0x55f, 0x55f, WBP::MidLetter},
  {0x560, 0x588, WBP::ALetter},
  {0x589, 0x589, WBP::MidNum},
  {0x58a, 0x590, WBP::ALetter},
  {0x591, 0x5bd, WBP::Extend},
  {0x5bf, 0x5bf, WBP::Extend},
  {0x5c1, 0x5c2, WBP::Extend},
  {0x5c4, 0x5c5, WBP::Extend},
  {0x5c7, 0x5c7, WBP::Extend},
  {0x5d0, 0x5ea, WBP::Hebrew_Letter},
  {0x5ef, 0x5f2, WBP::Hebrew_Letter},
  {0x5f3, 0x5f3, WBP::ALetter},
  {0x5f4, 0x5f4, WBP::MidLetter},
  {0x600, 0x605, WBP::Format},
  {0x60c, 0x60d, WBP::MidNum},
  {0x610, 0x61a, WBP::Extend},
  {0x61c, 0x61c, WBP::Format},
  {0x61d, 0x64a, WBP::ALetter},
  {0x64b, 0x65f, WBP::Extend},
  {0x660, 0x669, WBP::Numeric},
  {0x66b, 0x66b, WBP::Numeric},
  {0x66c, 0x66c, WBP::MidNum},
  {0x66d, 0x66f, WBP::ALetter},
  {0x670, 0x670, WBP::Extend},
  {0x671, 0x6d5, WBP::ALetter},
  {0x6d6, 0x6dc, WBP::Extend},
  {0x6dd, 0x6dd, WBP::Format},
  {0x6df, 0x6e4, WBP::Extend},
  {0x6e5, 0x6e6, WBP::ALetter},
  {0x6e7, 0x6e8, WBP::Extend},
  {0x6ea, 0x6ed, WBP::Extend},
  {0x6ee, 0x6ef, WBP::ALetter},
  {0x6f0, 0x6f9, WBP::Numeric},
  {0x6fa, 0x70e, WBP::ALetter},
  {0x70f, 0x70f, WBP::Format},
  {0x710, 0x710, WBP::ALetter},
  {0x711, 0x711, WBP::Extend},
  {0x712, 0x72f, WBP::ALetter},
  {0x730, 0x74a, WBP::Extend},
  {0x74b, 0x7a5, WBP::ALetter},
  {0x7a6, 0x7b0, WBP::Extend},
  {0x7b1, 0x7bf, WBP::ALetter},
  {0x7c0, 0x7c9, WBP::Numeric},
  {0x7ca, 0x7ea, WBP::ALetter},
  {0x7eb, 0x7f3, WBP::Extend},
  {0x7f4, 0x7f7, WBP::ALetter},
  {0x7f8, 0x7f8, WBP::MidNum},
  {0x7f9, 0x7fc, WBP::ALetter},
  {0x7fd, 0x7fd, WBP::Extend},
  {0x7fe, 0x815, WBP::ALetter},
  {0x816, 0x819, WBP::Extend},
  {0x81a, 0x81a, WBP::ALetter},
  {0x81b, 0x823, WBP::Extend},
  {0x824, 0x824, WBP::ALetter},
  {0x825, 0x827, WBP::Extend},
  {0x828, 0x828, WBP::ALetter},
  {0x829, 0x82d, WBP::Extend},
  {0x82e, 0x858, WBP::ALetter},
  {0x859, 0x85b, WBP::Extend},
  {0x85c, 0x8d2, WBP::ALetter},
  {0x8d3, 0x8e1, WBP::Extend},
  {0x8e2, 0x8e2, WBP::Format},
  {0x8e3, 0x903, WBP::Extend},
  {0x904, 0x939, WBP::ALetter},
  {0x93a, 0x93c, WBP::Extend},
  {0x93d, 0x93d, WBP::ALetter},
  {0x93e, 0x94f, WBP::Extend},
  {0x950, 0x950, WBP::ALetter},
  {0x951, 0x957, WBP::Extend},
  {0x958, 0x961, WBP::ALetter},
  {0x962, 0x963, WBP::Extend},
  {0x966, 0x96f, WBP::Numeric},
  {0x970, 0x980, WBP::ALetter},
  {0x981, 0x983, WBP::Extend},
  {0x984, 0x9bb, WBP::ALetter},
  {0x9bc, 0x9bc, WBP::Extend},
  {0x9bd, 0x9bd, WBP::ALetter},
  {0x9be, 0x9c4, WBP::Extend},
  {0x9c7, 0x9c8, WBP::Extend},
  {0x9cb, 0x9cd, WBP::Extend},
  {0x9ce, 0x9d6, WBP::ALetter},
  {0x9d7, 0x9d7, WBP::Extend},
  {0x9d8, 0x9e1, WBP::ALetter},
  {0x9e2, 0x9e3, WBP::Extend},
  {0x9e6, 0x9ef, WBP::Numeric},
  {0x9f0, 0x9fd, WBP::ALetter},
  {0x9fe, 0x9fe, WBP::Extend},
  {0xa01, 0xa03, WBP::Extend},
  {0xa04, 0xa3b, WBP::ALetter},
  {0xa3c, 0xa3c, WBP::Extend},
  {0xa3e, 0xa42, WBP::Extend},
  {0xa47, 0xa48, WBP::Extend},
  {0xa4b, 0xa4d, WBP::Extend},
  {0xa51, 0xa51, WBP::Extend},
  {0xa52, 0xa65, WBP::ALetter},
  {0xa66, 0xa6f, WBP::Numeric},
  {0xa70, 0xa71, WBP::Extend},
  {0xa72, 0xa74, WBP::ALetter},
  {0xa75, 0xa75, WBP::Extend},
  {0xa81, 0xa83, WBP::Extend},
  {0xa84, 0xabb, WBP::ALetter},
  {0xabc, 0xabc, WBP::Extend},
  {0xabd, 0xabd, WBP::ALetter},
  {0xabe, 0xac5, WBP::Extend},
  {0xac7, 0xac9, WBP::Extend},
  {0xacb, 0xacd, WBP::Extend},
  {0xace, 0xae1, WBP::ALetter},
  {0xae2, 0xae3, WBP::Extend},
  {0xae6, 0xaef, WBP::Numeric},
  {0xaf0, 0xaf9, WBP::ALetter},
  {0xafa, 0xaff, WBP::Extend},
  {0xb01, 0xb03, WBP::Extend},
  {0xb04, 0xb3b, WBP::ALetter},
  {0xb3c, 0xb3c, WBP::Extend},
  {0xb3d, 0xb3d, WBP::ALetter},
  {0xb3e, 0xb44, WBP::Extend},
  {0xb47, 0xb48, WBP::Extend},
  {0xb4b, 0xb4d, WBP::Extend},
  {0xb55, 0xb57, WBP::Extend},
  {0xb58, 0xb61, WBP::ALetter},
  {0xb62, 0xb63, WBP::Extend},
  {0xb66, 0xb6f, WBP::Numeric},
  {0xb70, 0xb81, WBP::ALetter},
  {0xb82, 0xb82, WBP::Extend},
  {0xb83, 0xbbd, WBP::ALetter},
  {0xbbe, 0xbc2, WBP::Extend},
  {0xbc6, 0xbc8, WBP::Extend},
  {0xbca, 0xbcd, WBP::Extend},
  {0xbce, 0xbd6, WBP::ALetter},
  {0xbd7, 0xbd7, WBP::Extend},
  {0xbe6, 0xbef, WBP::Numeric},
  {0xc00, 0xc04, WBP::Extend},
  {0xc05, 0xc3d, WBP::ALetter},
  {0xc3e, 0xc44, WBP::Extend},
  {0xc46, 0xc48, WBP::Extend},
  {0xc4a, 0xc4d, WBP::Extend},
  {0xc55, 0xc56, WBP::Extend},
  {0xc57, 0xc61, WBP::ALetter},
  {0xc62, 0xc63, WBP::Extend},
  {0xc66, 0xc6f, WBP::Numeric},
  {0xc70, 0xc80, WBP::ALetter},
  {0xc81, 0xc83, WBP::Extend},
  {0xc84, 0xcbb, WBP::ALetter},
  {0xcbc, 0xcbc, WBP::Extend},
  {0xcbd, 0xcbd, WBP::ALetter},
  {0xcbe, 0xcc4, WBP::Extend},
  {0xcc6, 0xcc8, WBP::Extend},
  {0xcca, 0xccd, WBP::Extend},
  {0xcd5, 0xcd6, WBP::Extend},
  {0xcd7, 0xce1, WBP::ALetter},
  {0xce2, 0xce3, WBP::Extend},
  {0xce6, 0xcef, WBP::Numeric},
  {0xcf0, 0xcff, WBP::ALetter},
  {0xd00, 0xd03, WBP::Extend},
  {0xd04, 0xd3a, WBP::ALetter},
  {0xd3b, 0xd3c, WBP::Extend},
  {0xd3d, 0xd3d, WBP::ALetter},
  {0xd3e, 0xd44, WBP::Extend},
  {0xd46, 0xd48, WBP::Extend},
  {0xd4a, 0xd4d, WBP::Extend},
  {0xd4e, 0xd56, WBP::ALetter},
  {0xd57, 0xd57, WBP::Extend},
  {0xd58, 0xd61, WBP::ALetter},
  {0xd62, 0xd63, WBP::Extend},
  {0xd66, 0xd6f, WBP::Numeric},
  {0xd70, 0xd80, WBP::ALetter},
  {0xd81, 0xd83, WBP::Extend},
  {0xd84, 0xdc9, WBP::ALetter},
  {0xdca, 0xdca, WBP::Extend},
  {0xdcf, 0xdd4, WBP::Extend},
  {0xdd6, 0xdd6, WBP::Extend},
  {0xdd8, 0xddf, WBP::Extend},
  {0xde6, 0xdef, WBP::Numeric},
  {0xdf2, 0xdf3, WBP::Extend},
  {0xe31, 0xe31, WBP::Extend},
  {0xe34, 0xe3a, WBP::Extend},
  {0xe47, 0xe4e, WBP::Extend},
  {0xe50, 0xe59, WBP::Numeric},
  {0xeb1, 0xeb1, WBP::Extend},
  {0xeb4, 0xebc, WBP::Extend},
  {0xec8, 0xecd, WBP::Extend},
  {0xed0, 0xed9, WBP::Numeric},
  {0xeda, 0xf17, WBP::ALetter},
  {0xf18, 0xf19, WBP::Extend},
  {0xf20, 0xf29, WBP::Numeric},
  {0xf35, 0xf35, WBP::Extend},
  {0xf37, 0xf37, WBP::Extend},
  {0xf39, 0xf39, WBP::Extend},
  {0xf3e, 0xf3f, WBP::Extend},
  {0xf40, 0xf70, WBP::ALetter},
  {0xf71, 0xf84, WBP::Extend},
  {0xf86, 0xf87, WBP::Extend},
  {0xf88, 0xf8c, WBP::ALetter},
  {0xf8d, 0xf97, WBP::Extend},
  {0xf99, 0xfbc, WBP::Extend},
  {0xfc6, 0xfc6, WBP::Extend},
  {0x102b, 0x103e, WBP::Extend},
  {0x1040, 0x1049, WBP::Numeric},
  {0x1056, 0x1059, WBP::Extend},
  {0x105e, 0x1060, WBP::Extend},
  {0x1062, 0x1064, WBP::Extend},
  {0x1067, 0x106d, WBP::Extend},
  {0x1071, 0x1074, WBP::Extend},
  {0x1082, 0x108d, WBP::Extend},
  {0x108f, 0x108f, WBP::Extend},
  {0x1090, 0x1099, WBP::Numeric},
  {0x109a, 0x109d, WBP::Extend},
  {0x109e, 0x135c, WBP::ALetter},
  {0x135d, 0x135f, WBP::Extend},
  {0x1360, 0x167f, WBP::ALetter},
  {0x1680, 0x1680, WBP::WSegSpace},
  {0x1681, 0x1711, WBP::ALetter},
  {0x1712, 0x1714, WBP::Extend},
  {0x1715, 0x1731, WBP::ALetter},
  {0x1732, 0x1734, WBP::Extend},
  {0x1735, 0x1751, WBP::ALetter},
  {0x1752, 0x1753, WBP::Extend},
  {0x1754, 0x1771, WBP::ALetter},
  {0x1772, 0x1773, WBP::Extend},
  {0x17b4, 0x17d3, WBP::Extend},
  {0x17dd, 0x17dd, WBP::Extend},
  {0x17e0, 0x17e9, WBP::Numeric},
  {0x180b, 0x180d, WBP::Extend},
  {0x180e, 0x180e, WBP::Format},
  {0x1810, 0x1819, WBP::Numeric},
  {0x181a, 0x1884, WBP::ALetter},
  {0x1885, 0x1886, WBP::Extend},
  {0x1887, 0x18a8, WBP::ALetter},
  {0x18a9, 0x18a9, WBP::Extend},
  {0x18aa, 0x191f, WBP::ALetter},
  {0x1920, 0x192b, WBP::Extend},
  {0x1930, 0x193b, WBP::Extend},
  {0x1946, 0x194f, WBP::Numeric},
  {0x19d0, 0x19d9, WBP::Numeric},
  {0x19da, 0x1a16, WBP::ALetter},
  {0x1a17, 0x1a1b, WBP::Extend},
  {0x1a55, 0x1a5e, WBP::Extend},
  {0x1a60, 0x1a7c, WBP::Extend},
  {0x1a7f, 0x1a7f, WBP::Extend},
  {0x1a80, 0x1a89, WBP::Numeric},
  {0x1a90, 0x1a99, WBP::Numeric},
  {0x1ab0, 0x1ac0, WBP::Extend},
  {0x1b00, 0x1b04, WBP::Extend},
  {0x1b05, 0x1b33, WBP::ALetter},
  {0x1b34, 0x1b44, WBP::Extend},
  {0x1b45, 0x1b4f, WBP::ALetter},
  {0x1b50, 0x1b59, WBP::Numeric},
  {0x1b6b, 0x1b73, WBP::Extend},
  {0x1b80, 0x1b82, WBP::Extend},
  {0x1b83, 0x1ba0, WBP::ALetter},
  {0x1ba1, 0x1bad, WBP::Extend},
  {0x1bae, 0x1baf, WBP::ALetter},
  {0x1bb0, 0x1bb9, WBP::Numeric},
  {0x1bba, 0x1be5, WBP::ALetter},
  {0x1be6, 0x1bf3, WBP::Extend},
  {0x1bf4, 0x1c23, WBP::ALetter},
  {0x1c24, 0x1c37, WBP::Extend},
  {0x1c40, 0x1c49, WBP::Numeric},
  {0x1c4a, 0x1c4f, WBP::ALetter},
  {0x1c50, 0x1c59, WBP::Numeric},
  {0x1c5a, 0x1ccf, WBP::ALetter},
  {0x1cd0, 0x1cd2, WBP::Extend},
  {0x1cd4, 0x1ce8, WBP::Extend},
  {0x1ce9, 0x1cec, WBP::ALetter},
  {0x1ced, 0x1ced, WBP::Extend},
  {0x1cee, 0x1cf3, WBP::ALetter},
  {0x1cf4, 0x1cf4, WBP::Extend},
  {0x1cf5, 0x1cf6, WBP::ALetter},
  {0x1cf7, 0x1cf9, WBP::Extend},
  {0x1cfa, 0x1dbf, WBP::ALetter},
  {0x1dc0, 0x1df9, WBP::Extend},
  {0x1dfb, 0x1dff, WBP::Extend},
  {0x1e00, 0x1fff, WBP::ALetter},
  {0x2000, 0x2006, WBP::WSegSpace},
  {0x2008, 0x200a, WBP::WSegSpace},
  {0x200c, 0x200c, WBP::Extend},
  {0x200d, 0x200d, WBP::ZWJ},
  {0x200e, 0x200f, WBP::Format},
  {0x2018, 0x2019, WBP::MidNumLet},
  {0x2024, 0x2024, WBP::MidNumLet},
  {0x2027, 0x2027, WBP::MidLetter},
  {0x2028, 0x2029, WBP::Newline},
  {0x202a, 0x202e, WBP::Format},
  {0x202f, 0x202f, WBP::ExtendNumLet},
  {0x203f, 0x2040, WBP::ExtendNumLet},
  {0x2044, 0x2044, WBP::MidNum},
  {0x2054, 0x2054, WBP::ExtendNumLet},
  {0x205f, 0x205f, WBP::WSegSpace},
  {0x2060, 0x2064, WBP::Format},
  {0x2066, 0x206f, WBP::Format},
  {0x2070, 0x20cf, WBP::ALetter},
  {0x20d0, 0x20f0, WBP::Extend},
  {0x20f1, 0x2cee, WBP::ALetter},
  {0x2cef, 0x2cf1, WBP::Extend},
  {0x2cf2, 0x2d7e, WBP::ALetter},
  {0x2d7f, 0x2d7f, WBP::Extend},
  {0x2d80, 0x2ddf, WBP::ALetter},
  {0x2de0, 0x2dff, WBP::Extend},
  {0x2e00, 0x2fff, WBP::ALetter},
  {0x3000, 0x3000, WBP::WSegSpace},
  {0x3001, 0x3029, WBP::ALetter},
  {0x302a, 0x302f, WBP::Extend},
  {0x3031, 0x3035, WBP::Katakana},
  {0x3036, 0x3098, WBP::ALetter},
  {0x3099, 0x309a, WBP::Extend},
  {0x309b, 0x309c, WBP::Katakana},
  {0x30a0, 0x30fa, WBP::Katakana},
  {0x30fc, 0x30ff, WBP::Katakana},
  {0x3100, 0x31ef, WBP::ALetter},
  {0x31f0, 0x31ff, WBP::Katakana},
  {0x32d0, 0x32fe, WBP::Katakana},
  {0x3300, 0x3357, WBP::Katakana},
  {0x3358, 0xa61f, WBP::ALetter},
  {0xa620, 0xa629, WBP::Numeric},
  {0xa62a, 0xa66e, WBP::ALetter},
  {0xa66f, 0xa672, WBP::Extend},
  {0xa674, 0xa67d, WBP::Extend},
  {0xa67e, 0xa69d, WBP::ALetter},
  {0xa69e, 0xa69f, WBP::Extend},
  {0xa6a0, 0xa6ef, WBP::ALetter},
  {0xa6f0, 0xa6f1, WBP::Extend},
  {0xa6f2, 0xa801, WBP::ALetter},
  {0xa802, 0xa802, WBP::Extend},
  {0xa803, 0xa805, WBP::ALetter},
  {0xa806, 0xa806, WBP::Extend},
  {0xa807, 0xa80a, WBP::ALetter},
  {0xa80b, 0xa80b, WBP::Extend},
  {0xa80c, 0xa822, WBP::ALetter},
  {0xa823, 0xa827, WBP::Extend},
  {0xa82c, 0xa82c, WBP::Extend},
  {0xa82d, 0xa87f, WBP::ALetter},
  {0xa880, 0xa881, WBP::Extend},
  {0xa882, 0xa8b3, WBP::ALetter},
  {0xa8b4, 0xa8c5, WBP::Extend},
  {0xa8d0, 0xa8d9, WBP::Numeric},
  {0xa8e0, 0xa8f1, WBP::Extend},
  {0xa8f2, 0xa8fe, WBP::ALetter},
  {0xa8ff, 0xa8ff, WBP::Extend},
  {0xa900, 0xa909, WBP::Numeric},
  {0xa90a, 0xa925, WBP::ALetter},
  {0xa926, 0xa92d, WBP::Extend},
  {0xa92e, 0xa946, WBP::ALetter},
  {0xa947, 0xa953, WBP::Extend},
  {0xa954, 0xa97f, WBP::ALetter},
  {0xa980, 0xa983, WBP::Extend},
  {0xa984, 0xa9b2, WBP::ALetter},
  {0xa9b3, 0xa9c0, WBP::Extend},
  {0xa9c1, 0xa9cf, WBP::ALetter},
  {0xa9d0, 0xa9d9, WBP::Numeric},
  {0xa9e5, 0xa9e5, WBP::Extend},
  {0xa9f0, 0xa9f9, WBP::Numeric},
  {0xa9fa, 0xaa28, WBP::ALetter},
  {0xaa29, 0xaa36, WBP::Extend},
  {0xaa37, 0xaa42, WBP::ALetter},
  {0xaa43, 0xaa43, WBP::Extend},
  {0xaa44, 0xaa4b, WBP::ALetter},
  {0xaa4c, 0xaa4d, WBP::Extend},
  {0xaa50, 0xaa59, WBP::Numeric},
  {0xaa7b, 0xaa7d, WBP::Extend},
  {0xaab0, 0xaab0, WBP::Extend},
  {0xaab2, 0xaab4, WBP::Extend},
  {0xaab7, 0xaab8, WBP::Extend},
  {0xaabe, 0xaabf, WBP::Extend},
  {0xaac1, 0xaac1, WBP::Extend},
  {0xaac2, 0xaaea, WBP::ALetter},
  {0xaaeb, 0xaaef, WBP::Extend},
  {0xaaf0, 0xaaf4, WBP::ALetter},
  {0xaaf5, 0xaaf6, WBP::Extend},
  {0xaaf7, 0xabe2, WBP::ALetter},
  {0xabe3, 0xabea, WBP::Extend},
  {0xabec, 0xabed, WBP::Extend},
  {0xabf0, 0xabf9, WBP::Numeric},
  {0xabfa, 0xfb1c, WBP::ALetter},
  {0xfb1d, 0xfb1d, WBP::Hebrew_Letter},
  {0xfb1e, 0xfb1e, WBP::Extend},
  {0xfb1f, 0xfb28, WBP::Hebrew_Letter},
  {0xfb2a, 0xfb36, WBP::Hebrew_Letter},
  {0xfb38, 0xfb3c, WBP::Hebrew_Letter},
  {0xfb3e, 0xfb3e, WBP::Hebrew_Letter},
  {0xfb40, 0xfb41, WBP::Hebrew_Letter},
  {0xfb43, 0xfb44, WBP::Hebrew_Letter},
  {0xfb46, 0xfb4f, WBP::Hebrew_Letter},
  {0xfb50, 0xfdff, WBP::ALetter},
  {0xfe00, 0xfe0f, WBP::Extend},
  {0xfe10, 0xfe10, WBP::MidNum},
  {0xfe13, 0xfe13, WBP::MidLetter},
  {0xfe14, 0xfe14, WBP::MidNum},
  {0xfe20, 0xfe2f, WBP::Extend},
  {0xfe33, 0xfe34, WBP::ExtendNumLet},
  {0xfe4d, 0xfe4f, WBP::ExtendNumLet},
  {0xfe50, 0xfe50, WBP::MidNum},
  {0xfe52, 0xfe52, WBP::MidNumLet},
  {0xfe54, 0xfe54, WBP::MidNum},
  {0xfe55, 0xfe55, WBP::MidLetter},
  {0xfe56, 0xfefe, WBP::ALetter},
  {0xfeff, 0xfeff, WBP::Format},
  {0xff07, 0xff07, WBP::MidNumLet},
  {0xff0c, 0xff0c, WBP::MidNum},
  {0xff0e, 0xff0e, WBP::MidNumLet},
  {0xff10, 0xff19, WBP::Numeric},
  {0xff1a, 0xff1a, WBP::MidLetter},
  {0xff1b, 0xff1b, WBP::MidNum},
  {0xff1c, 0xff3e, WBP::ALetter},
  {0xff3f, 0xff3f, WBP::ExtendNumLet},
  {0xff40, 0xff65, WBP::ALetter},
  {0xff66, 0xff9d, WBP::Katakana},
  {0xff9e, 0xff9f, WBP::Extend},
  {0xffa0, 0xfff8, WBP::ALetter},
  {0xfff9, 0xfffb, WBP::Format},
  {0xfffc, 0x101fc, WBP::ALetter},
  {0x101fd, 0x101fd, WBP::Extend},
  {0x101fe, 0x102df, WBP::ALetter},
  {0x102e0, 0x102e0, WBP::Extend},
  {0x102e1, 0x10375, WBP::ALetter},
  {0x10376, 0x1037a, WBP::Extend},
  {0x1037b, 0x1049f, WBP::ALetter},
  {0x104a0, 0x104a9, WBP::Numeric},
  {0x104aa, 0x10a00, WBP::ALetter},
  {0x10a01, 0x10a03, WBP::Extend},
  {0x10a05, 0x10a06, WBP::Extend},
  {0x10a0c, 0x10a0f, WBP::Extend},
  {0x10a10, 0x10a37, WBP::ALetter},
  {0x10a38, 0x10a3a, WBP::Extend},
  {0x10a3f, 0x10a3f, WBP::Extend},
  {0x10a40, 0x10ae4, WBP::ALetter},
  {0x10ae5, 0x10ae6, WBP::Extend},
  {0x10ae7, 0x10d23, WBP::ALetter},
  {0x10d24, 0x10d27, WBP::Extend},
  {0x10d30, 0x10d39, WBP::Numeric},
  {0x10d3a, 0x10eaa, WBP::ALetter},
  {0x10eab, 0x10eac, WBP::Extend},
  {0x10ead, 0x10f45, WBP::ALetter},
  {0x10f46, 0x10f50, WBP::Extend},
  {0x10f51, 0x10fff, WBP::ALetter},
  {0x11000, 0x11002, WBP::Extend},
  {0x11003, 0x11037, WBP::ALetter},
  {0x11038, 0x11046, WBP::Extend},
  {0x11066, 0x1106f, WBP::Numeric},
  {0x1107f, 0x11082, WBP::Extend},
  {0x11083, 0x110af, WBP::ALetter},
  {0x110b0, 0x110ba, WBP::Extend},
  {0x110bd, 0x110bd, WBP::Format},
  {0x110cd, 0x110cd, WBP::Format},
  {0x110ce, 0x110ef, WBP::ALetter},
  {0x110f0, 0x110f9, WBP::Numeric},
  {0x11100, 0x11102, WBP::Extend},
  {0x11103, 0x11126, WBP::ALetter},
  {0x11127, 0x11134, WBP::Extend},
  {0x11136, 0x1113f, WBP::Numeric},
  {0x11140, 0x11144, WBP::ALetter},
  {0x11145, 0x11146, WBP::Extend},
  {0x11147, 0x11172, WBP::ALetter},
  {0x11173, 0x11173, WBP::Extend},
  {0x11174, 0x1117f, WBP::ALetter},
  {0x11180, 0x11182, WBP::Extend},
  {0x11183, 0x111b2, WBP::ALetter},
  {0x111b3, 0x111c0, WBP::Extend},
  {0x111c1, 0x111c8, WBP::ALetter},
  {0x111c9, 0x111cc, WBP::Extend},
  {0x111ce, 0x111cf, WBP::Extend},
  {0x111d0, 0x111d9, WBP::Numeric},
  {0x111da, 0x1122b, WBP::ALetter},
  {0x1122c, 0x11237, WBP::Extend},
  {0x1123e, 0x1123e, WBP::Extend},
  {0x1123f, 0x112de, WBP::ALetter},
  {0x112df, 0x112ea, WBP::Extend},
  {0x112f0, 0x112f9, WBP::Numeric},
  {0x11300, 0x11303, WBP::Extend},
  {0x11304, 0x1133a, WBP::ALetter},
  {0x1133b, 0x1133c, WBP::Extend},
  {0x1133d, 0x1133d, WBP::ALetter},
  {0x1133e, 0x11344, WBP::Extend},
  {0x11347, 0x11348, WBP::Extend},
  {0x1134b, 0x1134d, WBP::Extend},
  {0x1134e, 0x11356, WBP::ALetter},
  {0x11357, 0x11357, WBP::Extend},
  {0x11358, 0x11361, WBP::ALetter},
  {0x11362, 0x11363, WBP::Extend},
  {0x11366, 0x1136c, WBP::Extend},
  {0x11370, 0x11374, WBP::Extend},
  {0x11375, 0x11434, WBP::ALetter},
  {0x11435, 0x11446, WBP::Extend},
  {0x11447, 0x1144f, WBP::ALetter},
  {0x11450, 0x11459, WBP::Numeric},
  {0x1145e, 0x1145e, WBP::Extend},
  {0x1145f, 0x114af, WBP::ALetter},
  {0x114b0, 0x114c3, WBP::Extend},
  {0x114c4, 0x114cf, WBP::ALetter},
  {0x114d0, 0x114d9, WBP::Numeric},
  {0x114da, 0x115ae, WBP::ALetter},
  {0x115af, 0x115b5, WBP::Extend},
  {0x115b8, 0x115c0, WBP::Extend},
  {0x115c1, 0x115db, WBP::ALetter},
  {0x115dc, 0x115dd, WBP::Extend},
  {0x115de, 0x1162f, WBP::ALetter},
  {0x11630, 0x11640, WBP::Extend},
  {0x11641, 0x1164f, WBP::ALetter},
  {0x11650, 0x11659, WBP::Numeric},
  {0x1165a, 0x116aa, WBP::ALetter},
  {0x116ab, 0x116b7, WBP::Extend},
  {0x116b8, 0x116bf, WBP::ALetter},
  {0x116c0, 0x116c9, WBP::Numeric},
  {0x1171d, 0x1172b, WBP::Extend},
  {0x11730, 0x11739, WBP::Numeric},
  {0x1173a, 0x1182b, WBP::ALetter},
  {0x1182c, 0x1183a, WBP::Extend},
  {0x1183b, 0x118df, WBP::ALetter},
  {0x118e0, 0x118e9, WBP::Numeric},
  {0x118ea, 0x1192f, WBP::ALetter},
  {0x11930, 0x11935, WBP::Extend},
  {0x11937, 0x11938, WBP::Extend},
  {0x1193b, 0x1193e, WBP::Extend},
  {0x1193f, 0x1193f, WBP::ALetter},
  {0x11940, 0x11940, WBP::Extend},
  {0x11941, 0x11941, WBP::ALetter},
  {0x11942, 0x11943, WBP::Extend},
  {0x11950, 0x11959, WBP::Numeric},
  {0x1195a, 0x119d0, WBP::ALetter},
  {0x119d1, 0x119d7, WBP::Extend},
  {0x119da, 0x119e0, WBP::Extend},
  {0x119e1, 0x119e3, WBP::ALetter},
  {0x119e4, 0x119e4, WBP::Extend},
  {0x119e5, 0x11a00, WBP::ALetter},
  {0x11a01, 0x11a0a, WBP::Extend},
  {0x11a0b, 0x11a32, WBP::ALetter},
  {0x11a33, 0x11a39, WBP::Extend},
  {0x11a3a, 0x11a3a, WBP::ALetter},
  {0x11a3b, 0x11a3e, WBP::Extend},
  {0x11a47, 0x11a47, WBP::Extend},
  {0x11a48, 0x11a50, WBP::ALetter},
  {0x11a51, 0x11a5b, WBP::Extend},
  {0x11a5c, 0x11a89, WBP::ALetter},
  {0x11a8a, 0x11a99, WBP::Extend},
  {0x11a9a, 0x11c2e, WBP::ALetter},
  {0x11c2f, 0x11c36, WBP::Extend},
  {0x11c38, 0x11c3f, WBP::Extend},
  {0x11c40, 0x11c4f, WBP::ALetter},
  {0x11c50, 0x11c59, WBP::Numeric},
  {0x11c5a, 0x11c91, WBP::ALetter},
  {0x11c92, 0x11ca7, WBP::Extend},
  {0x11ca9, 0x11cb6, WBP::Extend},
  {0x11cb7, 0x11d30, WBP::ALetter},
  {0x11d31, 0x11d36, WBP::Extend},
  {0x11d3a, 0x11d3a, WBP::Extend},
  {0x11d3c, 0x11d3d, WBP::Extend},
  {0x11d3f, 0x11d45, WBP::Extend},
  {0x11d46, 0x11d46, WBP::ALetter},
  {0x11d47, 0x11d47, WBP::Extend},
  {0x11d50, 0x11d59, WBP::Numeric},
  {0x11d5a, 0x11d89, WBP::ALetter},
  {0x11d8a, 0x11d8e, WBP::Extend},
  {0x11d90, 0x11d91, WBP::Extend},
  {0x11d93, 0x11d97, WBP::Extend},
  {0x11d98, 0x11d9f, WBP::ALetter},
  {0x11da0, 0x11da9, WBP::Numeric},
  {0x11daa, 0x11ef2, WBP::ALetter},
  {0x11ef3, 0x11ef6, WBP::Extend},
  {0x11ef7, 0x1342f, WBP::ALetter},
  {0x13430, 0x13438, WBP::Format},
  {0x13439, 0x16a5f, WBP::ALetter},
  {0x16a60, 0x16a69, WBP::Numeric},
  {0x16a6a, 0x16aef, WBP::ALetter},
  {0x16af0, 0x16af4, WBP::Extend},
  {0x16af5, 0x16b2f, WBP::ALetter},
  {0x16b30, 0x16b36, WBP::Extend},
  {0x16b37, 0x16b4f, WBP::ALetter},
  {0x16b50, 0x16b59, WBP::Numeric},
  {0x16b5a, 0x16f4e, WBP::ALetter},
  {0x16f4f, 0x16f4f, WBP::Extend},
  {0x16f50, 0x16f50, WBP::ALetter},
  {0x16f51, 0x16f87, WBP::Extend},
  {0x16f8f, 0x16f92, WBP::Extend},
  {0x16f93, 0x16fe3, WBP::ALetter},
  {0x16fe4, 0x16fe4, WBP::Extend},
  {0x16ff0, 0x16ff1, WBP::Extend},
  {0x1b000, 0x1b000, WBP::Katakana},
  {0x1b164, 0x1b167, WBP::Katakana},
  {0x1b168, 0x1bc9c, WBP::ALetter},
  {0x1bc9d, 0x1bc9e, WBP::Extend},
  {0x1bca0, 0x1bca3, WBP::Format},
  {0x1d165, 0x1d169, WBP::Extend},
  {0x1d16d, 0x1d172, WBP::Extend},
  {0x1d173, 0x1d17a, WBP::Format},
  {0x1d17b, 0x1d182, WBP::Extend},
  {0x1d185, 0x1d18b, WBP::Extend},
  {0x1d1aa, 0x1d1ad, WBP::Extend},
  {0x1d242, 0x1d244, WBP::Extend},
  {0x1d245, 0x1d7cd, WBP::ALetter},
  {0x1d7ce, 0x1d7ff, WBP::Numeric},
  {0x1da00, 0x1da36, WBP::Extend},
  {0x1da3b, 0x1da6c, WBP::Extend},
  {0x1da75, 0x1da75, WBP::Extend},
  {0x1da84, 0x1da84, WBP::Extend},
  {0x1da9b, 0x1da9f, WBP::Extend},
  {0x1daa1, 0x1daaf, WBP::Extend},
  {0x1e000, 0x1e006, WBP::Extend},
  {0x1e008, 0x1e018, WBP::Extend},
  {0x1e01b, 0x1e021, WBP::Extend},
  {0x1e023, 0x1e024, WBP::Extend},
  {0x1e026, 0x1e02a, WBP::Extend},
  {0x1e02b, 0x1e12f, WBP::ALetter},
  {0x1e130, 0x1e136, WBP::Extend},
  {0x1e137, 0x1e13f, WBP::ALetter},
  {0x1e140, 0x1e149, WBP::Numeric},
  {0x1e14a, 0x1e2eb, WBP::ALetter},
  {0x1e2ec, 0x1e2ef, WBP::Extend},
  {0x1e2f0, 0x1e2f9, WBP::Numeric},
  {0x1e2fa, 0x1e8cf, WBP::ALetter},
  {0x1e8d0, 0x1e8d6, WBP::Extend},
  {0x1e8d7, 0x1e943, WBP::ALetter},
  {0x1e944, 0x1e94a, WBP::Extend},
  {0x1e94b, 0x1e94f, WBP::ALetter},
  {0x1e950, 0x1e959, WBP::Numeric},
  {0x1e95a, 0x1f1e5, WBP::ALetter},
  {0x1f1e6, 0x1f1ff, WBP::Regional_Indicator},
  {0x1f3fb, 0x1f3ff, WBP::Extend},
  {0x1fbf0, 0x1fbf9, WBP::Numeric},
  {0xe0001, 0xe0001, WBP::Format},
  {0xe0020, 0xe007f, WBP::Extend},
  {0xe0100, 0xe01ef, WBP::Extend},
}};

// Find a codepoint inside a sorted list of Interval.
template <size_t N>
bool Bisearch(uint32_t ucs, const std::array<Interval, N>& table) {
  if (ucs < table.front().first || ucs > table.back().last) {  // NOLINT
    return false;
  }

  int min = 0;
  int max = N - 1;
  while (max >= min) {
    const int mid = (min + max) / 2;
    if (ucs > table[mid].last) {  // NOLINT
      min = mid + 1;
    } else if (ucs < table[mid].first) {  // NOLINT
      max = mid - 1;
    } else {
      return true;
    }
  }

  return false;
}

// Find a value inside a sorted list of Interval + property.
template <class C, size_t N>
bool Bisearch(uint32_t ucs, const std::array<C, N>& table, C* out) {
  int min = 0;
  int max = N - 1;
  while (max >= min) {
    const int mid = (min + max) / 2;
    if (ucs > table[mid].last) {  // NOLINT
      min = mid + 1;
    } else if (ucs < table[mid].first) {  // NOLINT
      max = mid - 1;
    } else {
      *out = table[mid];  // NOLINT
      return true;
    }
  }

  return false;
}

int codepoint_width(uint32_t ucs) {
  if (ftxui::IsControl(ucs)) {
    return -1;
  }

  if (ftxui::IsCombining(ucs)) {
    return 0;
  }

  if (ftxui::IsFullWidth(ucs)) {
    return 2;
  }

  return 1;
}

}  // namespace

namespace ftxui {

// From UTF8 encoded string |input|, eat in between 1 and 4 byte representing
// one codepoint. Put the codepoint into |ucs|. Start at |start| and update
// |end| to represent the beginning of the next byte to eat for consecutive
// executions.
bool EatCodePoint(const std::string& input,
                  size_t start,
                  size_t* end,
                  uint32_t* ucs) {
  if (start >= input.size()) {
    *end = start + 1;
    return false;
  }
  const uint8_t C0 = input[start];

  // 1 byte string.
  if ((C0 & 0b1000'0000) == 0b0000'0000) {  // NOLINT
    *ucs = C0 & 0b0111'1111;                // NOLINT
    *end = start + 1;
    return true;
  }

  // 2 byte string.
  if ((C0 & 0b1110'0000) == 0b1100'0000 &&  // NOLINT
      start + 1 < input.size()) {
    const uint8_t C1 = input[start + 1];
    *ucs = 0;
    *ucs += C0 & 0b0001'1111;  // NOLINT
    *ucs <<= 6;                // NOLINT
    *ucs += C1 & 0b0011'1111;  // NOLINT
    *end = start + 2;
    return true;
  }

  // 3 byte string.
  if ((C0 & 0b1111'0000) == 0b1110'0000 &&  // NOLINT
      start + 2 < input.size()) {
    const uint8_t C1 = input[start + 1];
    const uint8_t C2 = input[start + 2];
    *ucs = 0;
    *ucs += C0 & 0b0000'1111;  // NOLINT
    *ucs <<= 6;                // NOLINT
    *ucs += C1 & 0b0011'1111;  // NOLINT
    *ucs <<= 6;                // NOLINT
    *ucs += C2 & 0b0011'1111;  // NOLINT
    *end = start + 3;
    return true;
  }

  // 4 byte string.
  if ((C0 & 0b1111'1000) == 0b1111'0000 &&  // NOLINT
      start + 3 < input.size()) {
    const uint8_t C1 = input[start + 1];
    const uint8_t C2 = input[start + 2];
    const uint8_t C3 = input[start + 3];
    *ucs = 0;
    *ucs += C0 & 0b0000'0111;  // NOLINT
    *ucs <<= 6;                // NOLINT
    *ucs += C1 & 0b0011'1111;  // NOLINT
    *ucs <<= 6;                // NOLINT
    *ucs += C2 & 0b0011'1111;  // NOLINT
    *ucs <<= 6;                // NOLINT
    *ucs += C3 & 0b0011'1111;  // NOLINT
    *end = start + 4;
    return true;
  }

  *end = start + 1;
  return false;
}

// From UTF16 encoded string |input|, eat in between 1 and 4 byte representing
// one codepoint. Put the codepoint into |ucs|. Start at |start| and update
// |end| to represent the beginning of the next byte to eat for consecutive
// executions.
bool EatCodePoint(const std::wstring& input,
                  size_t start,
                  size_t* end,
                  uint32_t* ucs) {
  if (start >= input.size()) {
    *end = start + 1;
    return false;
  }

  // On linux wstring uses the UTF32 encoding:
  if constexpr (sizeof(wchar_t) == 4) {
    *ucs = input[start];  // NOLINT
    *end = start + 1;
    return true;
  }

  // On windows, wstring uses the UTF16 encoding:
  int32_t C0 = input[start];  // NOLINT

  // 1 word size:
  if (C0 < 0xd800 || C0 >= 0xdc00) {  // NOLINT
    *ucs = C0;
    *end = start + 1;
    return true;
  }

  // 2 word size:
  if (start + 1 >= input.size()) {
    *end = start + 2;
    return false;
  }

  int32_t C1 = input[start + 1];                         // NOLINT
  *ucs = ((C0 & 0x3ff) << 10) + (C1 & 0x3ff) + 0x10000;  // NOLINT
  *end = start + 2;
  return true;
}

bool IsCombining(uint32_t ucs) {
  return Bisearch(ucs, g_extend_characters);
}

bool IsFullWidth(uint32_t ucs) {
  if (ucs < 0x0300)  // Quick path: // NOLINT
    return false;

  return Bisearch(ucs, g_full_width_characters);
}

bool IsControl(uint32_t ucs) {
  if (ucs == 0) {
    return true;
  }
  if (ucs < 32) {  // NOLINT
    const uint32_t LINE_FEED = 10;
    return ucs != LINE_FEED;
  }
  if (ucs >= 0x7f && ucs < 0xa0) {  // NOLINT
    return true;
  }
  return false;
}

WordBreakProperty CodepointToWordBreakProperty(uint32_t codepoint) {
  WordBreakPropertyInterval interval = {0, 0, WBP::ALetter};
  std::ignore = Bisearch(codepoint, g_word_break_intervals, &interval);
  return interval.property;
}

int wchar_width(wchar_t ucs) {
  return codepoint_width(uint32_t(ucs));
}

int wstring_width(const std::wstring& text) {
  int width = 0;

  for (const wchar_t& it : text) {
    const int w = wchar_width(it);
    if (w < 0) {
      return -1;
    }
    width += w;
  }
  return width;
}

int string_width(const std::string& input) {
  int width = 0;
  size_t start = 0;
  while (start < input.size()) {
    uint32_t codepoint = 0;
    if (!EatCodePoint(input, start, &start, &codepoint)) {
      continue;
    }

    if (IsControl(codepoint)) {
      continue;
    }

    if (IsCombining(codepoint)) {
      continue;
    }

    if (IsFullWidth(codepoint)) {
      width += 2;
      continue;
    }

    width += 1;
  }
  return width;
}

std::vector<std::string> Utf8ToGlyphs(const std::string& input) {
  std::vector<std::string> out;
  const std::string current;
  out.reserve(input.size());
  size_t start = 0;
  size_t end = 0;
  while (start < input.size()) {
    uint32_t codepoint = 0;
    if (!EatCodePoint(input, start, &end, &codepoint)) {
      start = end;
      continue;
    }

    const std::string append = input.substr(start, end - start);
    start = end;

    // Ignore control characters.
    if (IsControl(codepoint)) {
      continue;
    }

    // Combining characters are put with the previous glyph they are modifying.
    if (IsCombining(codepoint)) {
      if (!out.empty()) {
        out.back() += append;
      }
      continue;
    }

    // Fullwidth characters take two cells. The second is made of the empty
    // string to reserve the space the first is taking.
    if (IsFullWidth(codepoint)) {
      out.push_back(append);
      out.emplace_back("");
      continue;
    }

    // Normal characters:
    out.push_back(append);
  }
  return out;
}

size_t GlyphPrevious(const std::string& input, size_t start) {
  while (true) {
    if (start == 0) {
      return 0;
    }
    start--;

    // Skip the UTF8 continuation bytes.
    if ((input[start] & 0b1100'0000) == 0b1000'0000) {
      continue;
    }

    uint32_t codepoint = 0;
    size_t end = 0;
    const bool eaten = EatCodePoint(input, start, &end, &codepoint);

    // Ignore invalid, control characters and combining characters.
    if (!eaten || IsControl(codepoint) || IsCombining(codepoint)) {
      continue;
    }

    return start;
  }
}

size_t GlyphNext(const std::string& input, size_t start) {
  bool glyph_found = false;
  while (start < input.size()) {
    size_t end = 0;
    uint32_t codepoint = 0;
    const bool eaten = EatCodePoint(input, start, &end, &codepoint);

    // Ignore invalid, control characters and combining characters.
    if (!eaten || IsControl(codepoint) || IsCombining(codepoint)) {
      start = end;
      continue;
    }

    // We eat the beginning of the next glyph. If we are eating the one
    // requested, return its start position immediately.
    if (glyph_found) {
      return static_cast<int>(start);
    }

    // Otherwise, skip this glyph and iterate:
    glyph_found = true;
    start = end;
  }
  return static_cast<int>(input.size());
}

size_t GlyphIterate(const std::string& input, int glyph_offset, size_t start) {
  if (glyph_offset >= 0) {
    for (int i = 0; i < glyph_offset; ++i) {
      start = GlyphNext(input, start);
    }
    return start;
  } else {
    for (int i = 0; i < -glyph_offset; ++i) {
      start = GlyphPrevious(input, start);
    }
    return start;
  }
}

std::vector<int> CellToGlyphIndex(const std::string& input) {
  int x = -1;
  std::vector<int> out;
  out.reserve(input.size());
  size_t start = 0;
  size_t end = 0;
  while (start < input.size()) {
    uint32_t codepoint = 0;
    const bool eaten = EatCodePoint(input, start, &end, &codepoint);
    start = end;

    // Ignore invalid / control characters.
    if (!eaten || IsControl(codepoint)) {
      continue;
    }

    // Combining characters are put with the previous glyph they are modifying.
    if (IsCombining(codepoint)) {
      if (x == -1) {
        ++x;
        out.push_back(x);
      }
      continue;
    }

    // Fullwidth characters take two cells. The second is made of the empty
    // string to reserve the space the first is taking.
    if (IsFullWidth(codepoint)) {
      ++x;
      out.push_back(x);
      out.push_back(x);
      continue;
    }

    // Normal characters:
    ++x;
    out.push_back(x);
  }
  return out;
}

int GlyphCount(const std::string& input) {
  int size = 0;
  size_t start = 0;
  size_t end = 0;
  while (start < input.size()) {
    uint32_t codepoint = 0;
    const bool eaten = EatCodePoint(input, start, &end, &codepoint);
    start = end;

    // Ignore invalid characters:
    if (!eaten || IsControl(codepoint)) {
      continue;
    }

    // Ignore combining characters, except when they don't have a preceding to
    // combine with.
    if (IsCombining(codepoint)) {
      if (size == 0) {
        size++;
      }
      continue;
    }

    size++;
  }
  return size;
}

std::vector<WordBreakProperty> Utf8ToWordBreakProperty(
    const std::string& input) {
  std::vector<WordBreakProperty> out;
  out.reserve(input.size());
  size_t start = 0;
  size_t end = 0;
  while (start < input.size()) {
    uint32_t codepoint = 0;
    if (!EatCodePoint(input, start, &end, &codepoint)) {
      start = end;
      continue;
    }
    start = end;

    // Ignore control characters.
    if (IsControl(codepoint)) {
      continue;
    }

    // Ignore combining characters.
    if (IsCombining(codepoint)) {
      continue;
    }

    WordBreakPropertyInterval interval = {0, 0, WBP::ALetter};
    std::ignore = Bisearch(codepoint, g_word_break_intervals, &interval);
    out.push_back(interval.property);
  }
  return out;
}

/// Convert a UTF8 std::string into a std::wstring.
std::string to_string(const std::wstring& s) {
  std::string out;

  size_t i = 0;
  uint32_t codepoint = 0;
  while (EatCodePoint(s, i, &i, &codepoint)) {
    // Code point <-> UTF-8 conversion
    //
    // ┏━━━━━━━━┳━━━━━━━━┳━━━━━━━━┳━━━━━━━━┓
    // ┃Byte 1  ┃Byte 2  ┃Byte 3  ┃Byte 4  ┃
    // ┡━━━━━━━━╇━━━━━━━━╇━━━━━━━━╇━━━━━━━━┩
    // │0xxxxxxx│        │        │        │
    // ├────────┼────────┼────────┼────────┤
    // │110xxxxx│10xxxxxx│        │        │
    // ├────────┼────────┼────────┼────────┤
    // │1110xxxx│10xxxxxx│10xxxxxx│        │
    // ├────────┼────────┼────────┼────────┤
    // │11110xxx│10xxxxxx│10xxxxxx│10xxxxxx│
    // └────────┴────────┴────────┴────────┘

    // 1 byte UTF8
    if (codepoint <= 0b000'0000'0111'1111) {  // NOLINT
      const uint8_t p1 = codepoint;
      out.push_back(p1);  // NOLINT
      continue;
    }

    // 2 bytes UTF8
    if (codepoint <= 0b000'0111'1111'1111) {  // NOLINT
      uint8_t p2 = codepoint & 0b111111;      // NOLINT
      codepoint >>= 6;                        // NOLINT
      uint8_t p1 = codepoint;                 // NOLINT
      out.push_back(0b11000000 + p1);         // NOLINT
      out.push_back(0b10000000 + p2);         // NOLINT
      continue;
    }

    // 3 bytes UTF8
    if (codepoint <= 0b1111'1111'1111'1111) {  // NOLINT
      uint8_t p3 = codepoint & 0b111111;       // NOLINT
      codepoint >>= 6;                         // NOLINT
      uint8_t p2 = codepoint & 0b111111;       // NOLINT
      codepoint >>= 6;                         // NOLINT
      uint8_t p1 = codepoint;                  // NOLINT
      out.push_back(0b11100000 + p1);          // NOLINT
      out.push_back(0b10000000 + p2);          // NOLINT
      out.push_back(0b10000000 + p3);          // NOLINT
      continue;
    }

    // 4 bytes UTF8
    if (codepoint <= 0b1'0000'1111'1111'1111'1111) {  // NOLINT
      uint8_t p4 = codepoint & 0b111111;              // NOLINT
      codepoint >>= 6;                                // NOLINT
      uint8_t p3 = codepoint & 0b111111;              // NOLINT
      codepoint >>= 6;                                // NOLINT
      uint8_t p2 = codepoint & 0b111111;              // NOLINT
      codepoint >>= 6;                                // NOLINT
      uint8_t p1 = codepoint;                         // NOLINT
      out.push_back(0b11110000 + p1);                 // NOLINT
      out.push_back(0b10000000 + p2);                 // NOLINT
      out.push_back(0b10000000 + p3);                 // NOLINT
      out.push_back(0b10000000 + p4);                 // NOLINT
      continue;
    }

    // Something else?
  }
  return out;
}

/// Convert a std::wstring into a UTF8 std::string.
std::wstring to_wstring(const std::string& s) {
  std::wstring out;

  size_t i = 0;
  uint32_t codepoint = 0;
  while (EatCodePoint(s, i, &i, &codepoint)) {
    // On linux wstring are UTF32 encoded:
    if constexpr (sizeof(wchar_t) == 4) {
      out.push_back(codepoint);  // NOLINT
      continue;
    }

    // On Windows, wstring are UTF16 encoded:

    // Codepoint encoded using 1 word:
    // NOLINTNEXTLINE
    if (codepoint < 0xD800 || (codepoint > 0xDFFF && codepoint < 0x10000)) {
      uint16_t p0 = codepoint;  // NOLINT
      out.push_back(p0);        // NOLINT
      continue;
    }

    // Codepoint encoded using 2 words:
    codepoint -= 0x010000;                               // NOLINT
    uint16_t p0 = (((codepoint << 12) >> 22) + 0xD800);  // NOLINT
    uint16_t p1 = (((codepoint << 22) >> 22) + 0xDC00);  // NOLINT
    out.push_back(p0);                                   // NOLINT
    out.push_back(p1);                                   // NOLINT
  }
  return out;
}

}  // namespace ftxui

// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
