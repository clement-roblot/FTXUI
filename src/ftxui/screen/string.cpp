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
static const std::array<WordBreakPropertyInterval, 993> g_word_break_intervals = {{
    {0x0000A, 0x0000A, WBP::LF},
    {0x0000B, 0x0000C, WBP::Newline},
    {0x0000D, 0x0000D, WBP::CR},
    {0x00020, 0x00020, WBP::WSegSpace},
    {0x00022, 0x00022, WBP::Double_Quote},
    {0x00027, 0x00027, WBP::Single_Quote},
    {0x0002C, 0x0002C, WBP::MidNum},
    {0x0002E, 0x0002E, WBP::MidNumLet},
    {0x00030, 0x00039, WBP::Numeric},
    {0x0003A, 0x0003A, WBP::MidLetter},
    {0x0003B, 0x0003B, WBP::MidNum},
    {0x0005F, 0x0005F, WBP::ExtendNumLet},
    {0x00085, 0x00085, WBP::Newline},
    {0x000AD, 0x000AD, WBP::Format},
    {0x000B7, 0x000B7, WBP::MidLetter},
    {0x00300, 0x0036F, WBP::Extend},
    {0x0037E, 0x0037E, WBP::MidNum},
    {0x00387, 0x00387, WBP::MidLetter},
    {0x00483, 0x00489, WBP::Extend},
    {0x0055F, 0x0055F, WBP::MidLetter},
    {0x00589, 0x00589, WBP::MidNum},
    {0x00591, 0x005BD, WBP::Extend},
    {0x005BF, 0x005BF, WBP::Extend},
    {0x005C1, 0x005C2, WBP::Extend},
    {0x005C4, 0x005C5, WBP::Extend},
    {0x005C7, 0x005C7, WBP::Extend},
    {0x005D0, 0x005EA, WBP::Hebrew_Letter},
    {0x005EF, 0x005F2, WBP::Hebrew_Letter},
    {0x005F4, 0x005F4, WBP::MidLetter},
    {0x00600, 0x00605, WBP::Format},
    {0x0060C, 0x0060D, WBP::MidNum},
    {0x00610, 0x0061A, WBP::Extend},
    {0x0061C, 0x0061C, WBP::Format},
    {0x0064B, 0x0065F, WBP::Extend},
    {0x00660, 0x00669, WBP::Numeric},
    {0x0066B, 0x0066B, WBP::Numeric},
    {0x0066C, 0x0066C, WBP::MidNum},
    {0x00670, 0x00670, WBP::Extend},
    {0x006D6, 0x006DC, WBP::Extend},
    {0x006DD, 0x006DD, WBP::Format},
    {0x006DF, 0x006E4, WBP::Extend},
    {0x006E7, 0x006E8, WBP::Extend},
    {0x006EA, 0x006ED, WBP::Extend},
    {0x006F0, 0x006F9, WBP::Numeric},
    {0x0070F, 0x0070F, WBP::Format},
    {0x00711, 0x00711, WBP::Extend},
    {0x00730, 0x0074A, WBP::Extend},
    {0x007A6, 0x007B0, WBP::Extend},
    {0x007C0, 0x007C9, WBP::Numeric},
    {0x007EB, 0x007F3, WBP::Extend},
    {0x007F8, 0x007F8, WBP::MidNum},
    {0x007FD, 0x007FD, WBP::Extend},
    {0x00816, 0x00819, WBP::Extend},
    {0x0081B, 0x00823, WBP::Extend},
    {0x00825, 0x00827, WBP::Extend},
    {0x00829, 0x0082D, WBP::Extend},
    {0x00859, 0x0085B, WBP::Extend},
    {0x008D3, 0x008E1, WBP::Extend},
    {0x008E2, 0x008E2, WBP::Format},
    {0x008E3, 0x00903, WBP::Extend},
    {0x0093A, 0x0093C, WBP::Extend},
    {0x0093E, 0x0094F, WBP::Extend},
    {0x00951, 0x00957, WBP::Extend},
    {0x00962, 0x00963, WBP::Extend},
    {0x00966, 0x0096F, WBP::Numeric},
    {0x00981, 0x00983, WBP::Extend},
    {0x009BC, 0x009BC, WBP::Extend},
    {0x009BE, 0x009C4, WBP::Extend},
    {0x009C7, 0x009C8, WBP::Extend},
    {0x009CB, 0x009CD, WBP::Extend},
    {0x009D7, 0x009D7, WBP::Extend},
    {0x009E2, 0x009E3, WBP::Extend},
    {0x009E6, 0x009EF, WBP::Numeric},
    {0x009FE, 0x009FE, WBP::Extend},
    {0x00A01, 0x00A03, WBP::Extend},
    {0x00A3C, 0x00A3C, WBP::Extend},
    {0x00A3E, 0x00A42, WBP::Extend},
    {0x00A47, 0x00A48, WBP::Extend},
    {0x00A4B, 0x00A4D, WBP::Extend},
    {0x00A51, 0x00A51, WBP::Extend},
    {0x00A66, 0x00A6F, WBP::Numeric},
    {0x00A70, 0x00A71, WBP::Extend},
    {0x00A75, 0x00A75, WBP::Extend},
    {0x00A81, 0x00A83, WBP::Extend},
    {0x00ABC, 0x00ABC, WBP::Extend},
    {0x00ABE, 0x00AC5, WBP::Extend},
    {0x00AC7, 0x00AC9, WBP::Extend},
    {0x00ACB, 0x00ACD, WBP::Extend},
    {0x00AE2, 0x00AE3, WBP::Extend},
    {0x00AE6, 0x00AEF, WBP::Numeric},
    {0x00AFA, 0x00AFF, WBP::Extend},
    {0x00B01, 0x00B03, WBP::Extend},
    {0x00B3C, 0x00B3C, WBP::Extend},
    {0x00B3E, 0x00B44, WBP::Extend},
    {0x00B47, 0x00B48, WBP::Extend},
    {0x00B4B, 0x00B4D, WBP::Extend},
    {0x00B55, 0x00B57, WBP::Extend},
    {0x00B62, 0x00B63, WBP::Extend},
    {0x00B66, 0x00B6F, WBP::Numeric},
    {0x00B82, 0x00B82, WBP::Extend},
    {0x00BBE, 0x00BC2, WBP::Extend},
    {0x00BC6, 0x00BC8, WBP::Extend},
    {0x00BCA, 0x00BCD, WBP::Extend},
    {0x00BD7, 0x00BD7, WBP::Extend},
    {0x00BE6, 0x00BEF, WBP::Numeric},
    {0x00C00, 0x00C04, WBP::Extend},
    {0x00C3E, 0x00C44, WBP::Extend},
    {0x00C46, 0x00C48, WBP::Extend},
    {0x00C4A, 0x00C4D, WBP::Extend},
    {0x00C55, 0x00C56, WBP::Extend},
    {0x00C62, 0x00C63, WBP::Extend},
    {0x00C66, 0x00C6F, WBP::Numeric},
    {0x00C81, 0x00C83, WBP::Extend},
    {0x00CBC, 0x00CBC, WBP::Extend},
    {0x00CBE, 0x00CC4, WBP::Extend},
    {0x00CC6, 0x00CC8, WBP::Extend},
    {0x00CCA, 0x00CCD, WBP::Extend},
    {0x00CD5, 0x00CD6, WBP::Extend},
    {0x00CE2, 0x00CE3, WBP::Extend},
    {0x00CE6, 0x00CEF, WBP::Numeric},
    {0x00D00, 0x00D03, WBP::Extend},
    {0x00D3B, 0x00D3C, WBP::Extend},
    {0x00D3E, 0x00D44, WBP::Extend},
    {0x00D46, 0x00D48, WBP::Extend},
    {0x00D4A, 0x00D4D, WBP::Extend},
    {0x00D57, 0x00D57, WBP::Extend},
    {0x00D62, 0x00D63, WBP::Extend},
    {0x00D66, 0x00D6F, WBP::Numeric},
    {0x00D81, 0x00D83, WBP::Extend},
    {0x00DCA, 0x00DCA, WBP::Extend},
    {0x00DCF, 0x00DD4, WBP::Extend},
    {0x00DD6, 0x00DD6, WBP::Extend},
    {0x00DD8, 0x00DDF, WBP::Extend},
    {0x00DE6, 0x00DEF, WBP::Numeric},
    {0x00DF2, 0x00DF3, WBP::Extend},
    {0x00E31, 0x00E31, WBP::Extend},
    {0x00E34, 0x00E3A, WBP::Extend},
    {0x00E47, 0x00E4E, WBP::Extend},
    {0x00E50, 0x00E59, WBP::Numeric},
    {0x00EB1, 0x00EB1, WBP::Extend},
    {0x00EB4, 0x00EBC, WBP::Extend},
    {0x00EC8, 0x00ECD, WBP::Extend},
    {0x00ED0, 0x00ED9, WBP::Numeric},
    {0x00F18, 0x00F19, WBP::Extend},
    {0x00F20, 0x00F29, WBP::Numeric},
    {0x00F35, 0x00F35, WBP::Extend},
    {0x00F37, 0x00F37, WBP::Extend},
    {0x00F39, 0x00F39, WBP::Extend},
    {0x00F3E, 0x00F3F, WBP::Extend},
    {0x00F71, 0x00F84, WBP::Extend},
    {0x00F86, 0x00F87, WBP::Extend},
    {0x00F8D, 0x00F97, WBP::Extend},
    {0x00F99, 0x00FBC, WBP::Extend},
    {0x00FC6, 0x00FC6, WBP::Extend},
    {0x0102B, 0x0103E, WBP::Extend},
    {0x01040, 0x01049, WBP::Numeric},
    {0x01056, 0x01059, WBP::Extend},
    {0x0105E, 0x01060, WBP::Extend},
    {0x01062, 0x01064, WBP::Extend},
    {0x01067, 0x0106D, WBP::Extend},
    {0x01071, 0x01074, WBP::Extend},
    {0x01082, 0x0108D, WBP::Extend},
    {0x0108F, 0x0108F, WBP::Extend},
    {0x01090, 0x01099, WBP::Numeric},
    {0x0109A, 0x0109D, WBP::Extend},
    {0x0135D, 0x0135F, WBP::Extend},
    {0x01680, 0x01680, WBP::WSegSpace},
    {0x01712, 0x01714, WBP::Extend},
    {0x01732, 0x01734, WBP::Extend},
    {0x01752, 0x01753, WBP::Extend},
    {0x01772, 0x01773, WBP::Extend},
    {0x017B4, 0x017D3, WBP::Extend},
    {0x017DD, 0x017DD, WBP::Extend},
    {0x017E0, 0x017E9, WBP::Numeric},
    {0x0180B, 0x0180D, WBP::Extend},
    {0x0180E, 0x0180E, WBP::Format},
    {0x01810, 0x01819, WBP::Numeric},
    {0x01885, 0x01886, WBP::Extend},
    {0x018A9, 0x018A9, WBP::Extend},
    {0x01920, 0x0192B, WBP::Extend},
    {0x01930, 0x0193B, WBP::Extend},
    {0x01946, 0x0194F, WBP::Numeric},
    {0x019D0, 0x019D9, WBP::Numeric},
    {0x01A17, 0x01A1B, WBP::Extend},
    {0x01A55, 0x01A5E, WBP::Extend},
    {0x01A60, 0x01A7C, WBP::Extend},
    {0x01A7F, 0x01A7F, WBP::Extend},
    {0x01A80, 0x01A89, WBP::Numeric},
    {0x01A90, 0x01A99, WBP::Numeric},
    {0x01AB0, 0x01AC0, WBP::Extend},
    {0x01B00, 0x01B04, WBP::Extend},
    {0x01B34, 0x01B44, WBP::Extend},
    {0x01B50, 0x01B59, WBP::Numeric},
    {0x01B6B, 0x01B73, WBP::Extend},
    {0x01B80, 0x01B82, WBP::Extend},
    {0x01BA1, 0x01BAD, WBP::Extend},
    {0x01BB0, 0x01BB9, WBP::Numeric},
    {0x01BE6, 0x01BF3, WBP::Extend},
    {0x01C24, 0x01C37, WBP::Extend},
    {0x01C40, 0x01C49, WBP::Numeric},
    {0x01C50, 0x01C59, WBP::Numeric},
    {0x01CD0, 0x01CD2, WBP::Extend},
    {0x01CD4, 0x01CE8, WBP::Extend},
    {0x01CED, 0x01CED, WBP::Extend},
    {0x01CF4, 0x01CF4, WBP::Extend},
    {0x01CF7, 0x01CF9, WBP::Extend},
    {0x01DC0, 0x01DF9, WBP::Extend},
    {0x01DFB, 0x01DFF, WBP::Extend},
    {0x02000, 0x02006, WBP::WSegSpace},
    {0x02008, 0x0200A, WBP::WSegSpace},
    {0x0200C, 0x0200C, WBP::Extend},
    {0x0200D, 0x0200D, WBP::ZWJ},
    {0x0200E, 0x0200F, WBP::Format},
    {0x02018, 0x02019, WBP::MidNumLet},
    {0x02024, 0x02024, WBP::MidNumLet},
    {0x02027, 0x02027, WBP::MidLetter},
    {0x02028, 0x02029, WBP::Newline},
    {0x0202A, 0x0202E, WBP::Format},
    {0x0202F, 0x0202F, WBP::ExtendNumLet},
    {0x0203F, 0x02040, WBP::ExtendNumLet},
    {0x02044, 0x02044, WBP::MidNum},
    {0x02054, 0x02054, WBP::ExtendNumLet},
    {0x0205F, 0x0205F, WBP::WSegSpace},
    {0x02060, 0x02064, WBP::Format},
    {0x02066, 0x0206F, WBP::Format},
    {0x020D0, 0x020F0, WBP::Extend},
    {0x02CEF, 0x02CF1, WBP::Extend},
    {0x02D7F, 0x02D7F, WBP::Extend},
    {0x02DE0, 0x02DFF, WBP::Extend},
    {0x03000, 0x03000, WBP::WSegSpace},
    {0x0302A, 0x0302F, WBP::Extend},
    {0x03031, 0x03035, WBP::Katakana},
    {0x03099, 0x0309A, WBP::Extend},
    {0x0309B, 0x0309C, WBP::Katakana},
    {0x030A0, 0x030FA, WBP::Katakana},
    {0x030FC, 0x030FF, WBP::Katakana},
    {0x031F0, 0x031FF, WBP::Katakana},
    {0x032D0, 0x032FE, WBP::Katakana},
    {0x03300, 0x03357, WBP::Katakana},
    {0x0A620, 0x0A629, WBP::Numeric},
    {0x0A66F, 0x0A672, WBP::Extend},
    {0x0A674, 0x0A67D, WBP::Extend},
    {0x0A69E, 0x0A69F, WBP::Extend},
    {0x0A6F0, 0x0A6F1, WBP::Extend},
    {0x0A802, 0x0A802, WBP::Extend},
    {0x0A806, 0x0A806, WBP::Extend},
    {0x0A80B, 0x0A80B, WBP::Extend},
    {0x0A823, 0x0A827, WBP::Extend},
    {0x0A82C, 0x0A82C, WBP::Extend},
    {0x0A880, 0x0A881, WBP::Extend},
    {0x0A8B4, 0x0A8C5, WBP::Extend},
    {0x0A8D0, 0x0A8D9, WBP::Numeric},
    {0x0A8E0, 0x0A8F1, WBP::Extend},
    {0x0A8FF, 0x0A8FF, WBP::Extend},
    {0x0A900, 0x0A909, WBP::Numeric},
    {0x0A926, 0x0A92D, WBP::Extend},
    {0x0A947, 0x0A953, WBP::Extend},
    {0x0A980, 0x0A983, WBP::Extend},
    {0x0A9B3, 0x0A9C0, WBP::Extend},
    {0x0A9D0, 0x0A9D9, WBP::Numeric},
    {0x0A9E5, 0x0A9E5, WBP::Extend},
    {0x0A9F0, 0x0A9F9, WBP::Numeric},
    {0x0AA29, 0x0AA36, WBP::Extend},
    {0x0AA43, 0x0AA43, WBP::Extend},
    {0x0AA4C, 0x0AA4D, WBP::Extend},
    {0x0AA50, 0x0AA59, WBP::Numeric},
    {0x0AA7B, 0x0AA7D, WBP::Extend},
    {0x0AAB0, 0x0AAB0, WBP::Extend},
    {0x0AAB2, 0x0AAB4, WBP::Extend},
    {0x0AAB7, 0x0AAB8, WBP::Extend},
    {0x0AABE, 0x0AABF, WBP::Extend},
    {0x0AAC1, 0x0AAC1, WBP::Extend},
    {0x0AAEB, 0x0AAEF, WBP::Extend},
    {0x0AAF5, 0x0AAF6, WBP::Extend},
    {0x0ABE3, 0x0ABEA, WBP::Extend},
    {0x0ABEC, 0x0ABED, WBP::Extend},
    {0x0ABF0, 0x0ABF9, WBP::Numeric},
    {0x0FB1D, 0x0FB1D, WBP::Hebrew_Letter},
    {0x0FB1E, 0x0FB1E, WBP::Extend},
    {0x0FB1F, 0x0FB28, WBP::Hebrew_Letter},
    {0x0FB2A, 0x0FB36, WBP::Hebrew_Letter},
    {0x0FB38, 0x0FB3C, WBP::Hebrew_Letter},
    {0x0FB3E, 0x0FB3E, WBP::Hebrew_Letter},
    {0x0FB40, 0x0FB41, WBP::Hebrew_Letter},
    {0x0FB43, 0x0FB44, WBP::Hebrew_Letter},
    {0x0FB46, 0x0FB4F, WBP::Hebrew_Letter},
    {0x0FE00, 0x0FE0F, WBP::Extend},
    {0x0FE10, 0x0FE10, WBP::MidNum},
    {0x0FE13, 0x0FE13, WBP::MidLetter},
    {0x0FE14, 0x0FE14, WBP::MidNum},
    {0x0FE20, 0x0FE2F, WBP::Extend},
    {0x0FE33, 0x0FE34, WBP::ExtendNumLet},
    {0x0FE4D, 0x0FE4F, WBP::ExtendNumLet},
    {0x0FE50, 0x0FE50, WBP::MidNum},
    {0x0FE52, 0x0FE52, WBP::MidNumLet},
    {0x0FE54, 0x0FE54, WBP::MidNum},
    {0x0FE55, 0x0FE55, WBP::MidLetter},
    {0x0FEFF, 0x0FEFF, WBP::Format},
    {0x0FF07, 0x0FF07, WBP::MidNumLet},
    {0x0FF0C, 0x0FF0C, WBP::MidNum},
    {0x0FF0E, 0x0FF0E, WBP::MidNumLet},
    {0x0FF10, 0x0FF19, WBP::Numeric},
    {0x0FF1A, 0x0FF1A, WBP::MidLetter},
    {0x0FF1B, 0x0FF1B, WBP::MidNum},
    {0x0FF3F, 0x0FF3F, WBP::ExtendNumLet},
    {0x0FF66, 0x0FF9D, WBP::Katakana},
    {0x0FF9E, 0x0FF9F, WBP::Extend},
    {0x0FFF9, 0x0FFFB, WBP::Format},
    {0x101FD, 0x101FD, WBP::Extend},
    {0x102E0, 0x102E0, WBP::Extend},
    {0x10376, 0x1037A, WBP::Extend},
    {0x104A0, 0x104A9, WBP::Numeric},
    {0x10A01, 0x10A03, WBP::Extend},
    {0x10A05, 0x10A06, WBP::Extend},
    {0x10A0C, 0x10A0F, WBP::Extend},
    {0x10A38, 0x10A3A, WBP::Extend},
    {0x10A3F, 0x10A3F, WBP::Extend},
    {0x10AE5, 0x10AE6, WBP::Extend},
    {0x10D24, 0x10D27, WBP::Extend},
    {0x10D30, 0x10D39, WBP::Numeric},
    {0x10EAB, 0x10EAC, WBP::Extend},
    {0x10F46, 0x10F50, WBP::Extend},
    {0x11000, 0x11002, WBP::Extend},
    {0x11038, 0x11046, WBP::Extend},
    {0x11066, 0x1106F, WBP::Numeric},
    {0x1107F, 0x11082, WBP::Extend},
    {0x110B0, 0x110BA, WBP::Extend},
    {0x110BD, 0x110BD, WBP::Format},
    {0x110CD, 0x110CD, WBP::Format},
    {0x110F0, 0x110F9, WBP::Numeric},
    {0x11100, 0x11102, WBP::Extend},
    {0x11127, 0x11134, WBP::Extend},
    {0x11136, 0x1113F, WBP::Numeric},
    {0x11145, 0x11146, WBP::Extend},
    {0x11173, 0x11173, WBP::Extend},
    {0x11180, 0x11182, WBP::Extend},
    {0x111B3, 0x111C0, WBP::Extend},
    {0x111C9, 0x111CC, WBP::Extend},
    {0x111CE, 0x111CF, WBP::Extend},
    {0x111D0, 0x111D9, WBP::Numeric},
    {0x1122C, 0x11237, WBP::Extend},
    {0x1123E, 0x1123E, WBP::Extend},
    {0x112DF, 0x112EA, WBP::Extend},
    {0x112F0, 0x112F9, WBP::Numeric},
    {0x11300, 0x11303, WBP::Extend},
    {0x1133B, 0x1133C, WBP::Extend},
    {0x1133E, 0x11344, WBP::Extend},
    {0x11347, 0x11348, WBP::Extend},
    {0x1134B, 0x1134D, WBP::Extend},
    {0x11357, 0x11357, WBP::Extend},
    {0x11362, 0x11363, WBP::Extend},
    {0x11366, 0x1136C, WBP::Extend},
    {0x11370, 0x11374, WBP::Extend},
    {0x11435, 0x11446, WBP::Extend},
    {0x11450, 0x11459, WBP::Numeric},
    {0x1145E, 0x1145E, WBP::Extend},
    {0x114B0, 0x114C3, WBP::Extend},
    {0x114D0, 0x114D9, WBP::Numeric},
    {0x115AF, 0x115B5, WBP::Extend},
    {0x115B8, 0x115C0, WBP::Extend},
    {0x115DC, 0x115DD, WBP::Extend},
    {0x11630, 0x11640, WBP::Extend},
    {0x11650, 0x11659, WBP::Numeric},
    {0x116AB, 0x116B7, WBP::Extend},
    {0x116C0, 0x116C9, WBP::Numeric},
    {0x1171D, 0x1172B, WBP::Extend},
    {0x11730, 0x11739, WBP::Numeric},
    {0x1182C, 0x1183A, WBP::Extend},
    {0x118E0, 0x118E9, WBP::Numeric},
    {0x11930, 0x11935, WBP::Extend},
    {0x11937, 0x11938, WBP::Extend},
    {0x1193B, 0x1193E, WBP::Extend},
    {0x11940, 0x11940, WBP::Extend},
    {0x11942, 0x11943, WBP::Extend},
    {0x11950, 0x11959, WBP::Numeric},
    {0x119D1, 0x119D7, WBP::Extend},
    {0x119DA, 0x119E0, WBP::Extend},
    {0x119E4, 0x119E4, WBP::Extend},
    {0x11A01, 0x11A0A, WBP::Extend},
    {0x11A33, 0x11A39, WBP::Extend},
    {0x11A3B, 0x11A3E, WBP::Extend},
    {0x11A47, 0x11A47, WBP::Extend},
    {0x11A51, 0x11A5B, WBP::Extend},
    {0x11A8A, 0x11A99, WBP::Extend},
    {0x11C2F, 0x11C36, WBP::Extend},
    {0x11C38, 0x11C3F, WBP::Extend},
    {0x11C50, 0x11C59, WBP::Numeric},
    {0x11C92, 0x11CA7, WBP::Extend},
    {0x11CA9, 0x11CB6, WBP::Extend},
    {0x11D31, 0x11D36, WBP::Extend},
    {0x11D3A, 0x11D3A, WBP::Extend},
    {0x11D3C, 0x11D3D, WBP::Extend},
    {0x11D3F, 0x11D45, WBP::Extend},
    {0x11D47, 0x11D47, WBP::Extend},
    {0x11D50, 0x11D59, WBP::Numeric},
    {0x11D8A, 0x11D8E, WBP::Extend},
    {0x11D90, 0x11D91, WBP::Extend},
    {0x11D93, 0x11D97, WBP::Extend},
    {0x11DA0, 0x11DA9, WBP::Numeric},
    {0x11EF3, 0x11EF6, WBP::Extend},
    {0x13430, 0x13438, WBP::Format},
    {0x16A60, 0x16A69, WBP::Numeric},
    {0x16AF0, 0x16AF4, WBP::Extend},
    {0x16B30, 0x16B36, WBP::Extend},
    {0x16B50, 0x16B59, WBP::Numeric},
    {0x16F4F, 0x16F4F, WBP::Extend},
    {0x16F51, 0x16F87, WBP::Extend},
    {0x16F8F, 0x16F92, WBP::Extend},
    {0x16FE4, 0x16FE4, WBP::Extend},
    {0x16FF0, 0x16FF1, WBP::Extend},
    {0x1B000, 0x1B000, WBP::Katakana},
    {0x1B164, 0x1B167, WBP::Katakana},
    {0x1BC9D, 0x1BC9E, WBP::Extend},
    {0x1BCA0, 0x1BCA3, WBP::Format},
    {0x1D165, 0x1D169, WBP::Extend},
    {0x1D16D, 0x1D172, WBP::Extend},
    {0x1D173, 0x1D17A, WBP::Format},
    {0x1D17B, 0x1D182, WBP::Extend},
    {0x1D185, 0x1D18B, WBP::Extend},
    {0x1D1AA, 0x1D1AD, WBP::Extend},
    {0x1D242, 0x1D244, WBP::Extend},
    {0x1D7CE, 0x1D7FF, WBP::Numeric},
    {0x1DA00, 0x1DA36, WBP::Extend},
    {0x1DA3B, 0x1DA6C, WBP::Extend},
    {0x1DA75, 0x1DA75, WBP::Extend},
    {0x1DA84, 0x1DA84, WBP::Extend},
    {0x1DA9B, 0x1DA9F, WBP::Extend},
    {0x1DAA1, 0x1DAAF, WBP::Extend},
    {0x1E000, 0x1E006, WBP::Extend},
    {0x1E008, 0x1E018, WBP::Extend},
    {0x1E01B, 0x1E021, WBP::Extend},
    {0x1E023, 0x1E024, WBP::Extend},
    {0x1E026, 0x1E02A, WBP::Extend},
    {0x1E130, 0x1E136, WBP::Extend},
    {0x1E140, 0x1E149, WBP::Numeric},
    {0x1E2EC, 0x1E2EF, WBP::Extend},
    {0x1E2F0, 0x1E2F9, WBP::Numeric},
    {0x1E8D0, 0x1E8D6, WBP::Extend},
    {0x1E944, 0x1E94A, WBP::Extend},
    {0x1E950, 0x1E959, WBP::Numeric},
    {0x1F1E6, 0x1F1FF, WBP::Regional_Indicator},
    {0x1F3FB, 0x1F3FF, WBP::Extend},
    {0x1FBF0, 0x1FBF9, WBP::Numeric},
    {0xE0001, 0xE0001, WBP::Format},
    {0xE0020, 0xE007F, WBP::Extend},
    {0xE0100, 0xE01EF, WBP::Extend},
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
