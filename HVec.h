#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <string>

#define HVecT HVec
#define xstrgf(s) strgf(s)
#define strgf(s) #s

template<unsigned dim>
struct HVecT
{
  std::array<float, dim> d;
  template<typename... T>
  HVecT(T&&... args)
    : d{ args... }
  {}

  float operator[](unsigned n) const { return d[n]; }
  unsigned size(void) const;
  float vlen(void) const;
  bool operator<(const HVecT& b) const;
  constexpr bool operator>(const HVecT& b) { return b < (*this); }
  constexpr bool operator<=(const HVecT& b) { return !((*this) > b); }
  constexpr bool operator>=(const HVecT& b) { return !((*this) < b); }
  bool operator==(const HVecT& b) const;
  constexpr bool operator!=(const HVecT& b) { return !((*this) == b); }
  // constexpr operator<=>(const HVecT& b) {
  //  return (*this) < b ? std::weak_ordering::less :
  //    b < (*this) ? std::weak_ordering::greater :
  //    std::weak_ordering::equivalent;
  //}
  auto sum(const HVecT& b) const;
  auto mul(const float a);
  auto norm(void);
  float dprod(const HVecT& b) const;
  auto cprod(const HVecT& b) const;

  std::string to_string(void);
};

template<unsigned dim>
inline unsigned
HVecT<dim>::size(void) const
{
  return dim;
}

// Длина вектора
template<unsigned dim>
inline float
HVecT<dim>::vlen(void) const
{
  auto v = std::array<float, dim>(this->d);
  std::transform(
    v.begin(), v.end(), v.begin(), [](float& n) { return (n * n); });
  return sqrtf(std::accumulate(v.begin(), v.end(), 0.0f));
}

template<unsigned dim>
inline bool
HVecT<dim>::operator<(const HVecT& b) const
{
  return (*this).vlen() < b.vlen();
}

template<unsigned dim>
inline bool
HVecT<dim>::operator==(const HVecT& b) const
{
  return (*this).vlen() == b.vlen();
}

// Векторное сложение
template<unsigned dim>
inline auto
HVecT<dim>::sum(const HVecT& b) const
{
  throw std::logic_error("Function not yet implemented");
} // TODO implementation

// Умножение вектора на скаляр
template<unsigned dim>
inline auto
HVecT<dim>::mul(const float a)
{
  std::transform((this->d).begin(),
                 (this->d).end(),
                 (this->d).begin(),
                 [a](float& n) { return (n * a); });
}

// Нормализация вектора
template<unsigned dim>
inline auto
HVecT<dim>::norm(void)
{
  auto v = std::array<float, dim>(this->d);
  std::transform(v.begin(), v.end(), v.begin(), [](float& n) {
    return (n > 0.0f) ? (n) : (-n);
  });
  (*this).mul(1/(*std::max_element(v.begin(), v.end())));
}

// Скалярное произведение векторов
template<unsigned dim>
inline float
HVecT<dim>::dprod(const HVecT& b) const
{
  throw std::logic_error("Function not yet implemented");
} // TODO implementation

// Векторное произведение
template<unsigned dim>
auto
HVecT<dim>::cprod(const HVecT<dim>& b) const
{
  return cprod(*this, b);
}

// Векторное произведение (размерность 2)
static inline float
cprod(const HVecT<2>& d, const HVecT<2>& b)
{
  return d[0] * b[1] - d[1] * b[0];
}

// Векторное произведение (размерность 2)
static inline HVecT<3>
cprod(const HVecT<3>& d, const HVecT<3>& b)
{
  return HVecT<3>{ d[1] * b[2] - d[2] * b[1],
                   d[2] * b[0] - d[0] * b[2],
                   d[0] * b[1] - d[1] * b[0] };
}

// Сортировка Шелла (по последовательности Циура)
template<typename T>
void
shell_sort(T first, T last)
{
  static const unsigned gaps[16] = { 510774, 227011, 100894, 44842, 19930, 8858,
                                     3937,   1750,   701,    301,   132,   57,
                                     23,     10,     4,      1 };
  for (int x = 0; x < 16; ++x) {
    unsigned int d = gaps[x];
    if (d > last - first)
      continue;
    for (auto i = first + d; i != last; ++i)
      for (auto j = i; j - first >= d && *j > *(j - d); j -= d)
        std::swap(*j, *(j - d));
  }
}

// Строковое представление вектора
template<unsigned dim>
std::string
HVecT<dim>::to_string(void)
{
  auto s = std::string(xstrgf(HVecT));
  s += "<" + std::to_string(dim) + ">{" +
       std::accumulate(std::next((this->d).begin()),
                       (this->d).end(),
                       std::to_string((this->d)[0]),
                       [](std::string a, float b) {
                         return std::move(a) + ' ' + std::to_string(b);
                       }) +
       "}";

  return s;
}

#undef strgf
#undef xstrgf
#undef HVecT