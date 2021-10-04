#include "HVec.h"

#pragma region TEST_MACROS
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

#define TESTSUP()                                                              \
  std::string _test_info;                                                      \
  signed _test_marker = 0;                                                     \
  unsigned _test_assertline, _tests_passed = 0, _tests_failed = 0,             \
                             _tests_skipped = 0;                               \
  bool _test_shouldfail = false, _fm = false;                                  \
  std::chrono::system_clock::time_point _test_start, _test_end;                \
  std::chrono::duration<double, std::milli> _test_span

#define TEST(x, y)                                                             \
  _fm = false;                                                                 \
  _test_info = x;                                                              \
  _test_assertline = 0;                                                        \
  _test_shouldfail = !y;                                                       \
  _test_marker = 0;                                                            \
  _test_start = std::chrono::system_clock::now();                              \
  do {                                                                         \
    switch (_test_marker) {                                                    \
      case 0: {

#define TESTEND                                                                \
  }                                                                            \
  _test_marker = 1;                                                            \
  break;                                                                       \
  case -1 /*FAIL*/:                                                            \
    _fm = _test_shouldfail;                                                    \
    if (_test_shouldfail) {                                                    \
      _test_marker = 1;                                                        \
      _fm = _test_shouldfail;                                                  \
      _test_shouldfail = !_test_shouldfail;                                    \
      break;                                                                   \
    }                                                                          \
    _test_end = std::chrono::system_clock::now();                              \
    _test_span = _test_end - _test_start;                                      \
    std::cout << "FAILED   " << std::left << std::setw(26) << _test_info       \
              << "\t@ " << std::setw(8)                                        \
              << (_test_assertline ? (std::to_string(_test_assertline))        \
                                   : (std::string("?")))                       \
              << std::right << std::setw(10) << _test_span.count() << "ms "    \
              << (!_fm ? (std::string("(passing)\n"))                          \
                       : (std::string("(failing)\n")));                        \
    _tests_failed++;                                                           \
    _test_marker = -2;                                                         \
    break;                                                                     \
  case 1 /*OK*/:                                                               \
    if (_test_shouldfail) {                                                    \
      _test_marker = -1;                                                       \
      _fm = _test_shouldfail;                                                  \
      _test_shouldfail = !_test_shouldfail;                                    \
      break;                                                                   \
    }                                                                          \
    _test_end = std::chrono::system_clock::now();                              \
    _test_span = _test_end - _test_start;                                      \
    std::cout << "OK       " << std::left << std::setw(26) << _test_info       \
              << "\t@ " << std::setw(8)                                        \
              << (_test_assertline ? (std::to_string(_test_assertline))        \
                                   : (std::string("?")))                       \
              << std::right << std::setw(10) << _test_span.count() << "ms "    \
              << (!_fm ? (std::string("(passing)\n"))                          \
                       : (std::string("(failing)\n")));                        \
    _tests_passed++;                                                           \
    _test_marker = -2;                                                         \
    break;                                                                     \
  case 2 /*SKIP*/:                                                             \
    std::cout << "SKIPPED  " << std::left << std::setw(26) << _test_info       \
              << "\t@ "                                                        \
              << (_test_assertline ? (std::to_string(_test_assertline))        \
                                   : (std::string("?")))                       \
              << "\n";                                                         \
    _tests_skipped++;                                                          \
    _test_marker = -2;                                                         \
    break;                                                                     \
    }                                                                          \
    }                                                                          \
    while (_test_marker != -2)                                                 \
      ;

#define SKIP(x)                                                                \
  if (x) {                                                                     \
    _test_assertline = __LINE__;                                               \
    _test_marker = 2;                                                          \
    break;                                                                     \
  }
#define ASSERT(x)                                                              \
  if (!(x)) {                                                                  \
    _test_assertline = __LINE__;                                               \
    _test_marker = -1;                                                         \
    break;                                                                     \
  }
#define REPORT()                                                               \
  std::cout << "passed " << _tests_passed << ",\tfailed " << _tests_failed     \
            << ",\tskipped " << _tests_skipped << "\n"
#pragma endregion TEST_MACROS

int
main()
{
  TESTSUP();

  TEST("vec0", false);
  auto vec0 = new HVec<0>();
  ASSERT(vec0 == nullptr);
  TESTEND;
  TEST("vec2", true);
  auto vec2 = new HVec<2>();
  ASSERT(vec2 != nullptr);
  TESTEND;
  TEST("vec3", true);
  auto vec3 = new HVec<3>();
  ASSERT(vec3 != nullptr);
  TESTEND;
  TEST("vec3 initlist", true);
  auto vec3 = new HVec<3>{ .1f, .2f, .3f };
  ASSERT(vec3 != nullptr);
  TESTEND;
  TEST("vec[]", true);
  auto vec3 = HVec<3>{ .1f, .2f, .3f };
  ASSERT(vec3[1] == .2f);
  TESTEND;
  TEST("vec3.vlen", true);
  auto vec3a = HVec<3>{ 1.0f, 2.0f, 3.0f };
  ASSERT((vec3a.vlen() - 3.742f) < 0.00001);
  auto vec3b = HVec<3>{ 0.0f, 0.0f, 1.0f };
  ASSERT((vec3b.vlen() - 1.0f) < 0.00001);
  TESTEND;

  TEST("op == self", true);
  auto vec3 = HVec<3>{ .1f, .2f, .3f };
  ASSERT(vec3 == vec3);
  TESTEND;
  TEST("op ==", true);
  auto vec3a = HVec<3>{ .1f, .2f, .3f };
  auto vec3b = HVec<3>{ .1f, .2f, .3f };
  ASSERT(vec3a == vec3b);
  TESTEND;
  TEST("op !=", true);
  auto vec3a = HVec<3>{ .1f, .2f, .3f };
  auto vec3b = HVec<3>{ .0f, .0f, .0f };
  ASSERT(vec3b != vec3a);
  TESTEND;

  TEST("op <", true);
  auto vec3a = HVec<3>{ .1f, .2f, .3f };
  auto vec3b = HVec<3>{ .1f, .2f, .1f };
  ASSERT(vec3b < vec3a);
  TESTEND;
  TEST("op < alt", false);
  auto vec3a = HVec<3>{ .1f, .2f, .3f };
  auto vec3b = HVec<3>{ .0f, .0f, .0f };
  ASSERT(vec3a < vec3b);
  TESTEND;

  TEST("op >", true);
  auto vec3a = HVec<3>{ .1f, -.2f, -.3f };
  auto vec3b = HVec<3>{ .1f, .1f, .0f };
  ASSERT(vec3a > vec3b);
  TESTEND;
  TEST("op >=", true);
  auto vec3a = HVec<3>{ .1f, .2f, .3f };
  auto vec3b = HVec<3>{ .0f, .0f, .3f };
  auto vec3c = HVec<3>{ .0f, .0f, .3f };
  ASSERT(vec3a >= vec3b);
  ASSERT(vec3c >= vec3b);
  TESTEND;
  TEST("op <=", true);
  auto vec3a = HVec<3>{ .1f, .2f, .3f };
  auto vec3b = HVec<3>{ .0f, .0f, .0f };
  auto vec3c = HVec<3>{ .1f, .2f, .3f };
  ASSERT(vec3b <= vec3a);
  ASSERT(vec3b <= vec3a);
  TESTEND;

  TEST("to_string", true);
  auto vec3a = HVec<3>{ .1f, .2f, .3f };
  ASSERT(vec3a.to_string() == "HVec<3>{0.100000 0.200000 0.300000}");
  TESTEND;

  TEST("shell_sort", true);
  auto vecs = std::array<HVec<3>, 5>{
    HVec<3>{ .1f, .2f, .3f },  HVec<3>{ .6f, .3f, .3f },
    HVec<3>{ .1f, -.7f, .1f }, HVec<3>{ -.1f, -.2f, .0f },
    HVec<3>{ .0f, .5f, .4f },
  };
  shell_sort(vecs.begin(), vecs.end());
  auto s = std::accumulate(
    std::next(vecs.begin()),
    vecs.end(),
    vecs[0].to_string(),
    [](auto a, auto& b) { return std::move(a) + ' ' + b.to_string(); });
  ASSERT(s ==
         std::string(
           "HVec<3>{0.600000 0.300000 0.300000} HVec<3>{0.100000 -0.700000 "
           "0.100000} HVec<3>{0.000000 0.500000 0.400000} HVec<3>{0.100000 "
           "0.200000 0.300000} HVec<3>{-0.100000 -0.200000 0.000000}"));
  TESTEND;

  REPORT();
  return _tests_failed != 0;
}
