#include "Vec2.h"

#include <cmath>

Vec2::Vec2()
{
}

Vec2::Vec2(float xin, float yin) : x(xin), y(yin)
{
}

Vec2 Vec2::operator+(const Vec2 &rhs) const
{
  return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2 &rhs) const
{
  return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator/(const float val) const
{
  return Vec2(x / val, y / val);
}

Vec2 Vec2::operator*(const float val) const
{
  return Vec2(x * val, y * val);
}

Vec2 Vec2::normalized(const Vec2 &vec) const
{
  float magnitude = std::sqrt(vec.x * vec.x + vec.y * vec.y);
  Vec2 normalized = Vec2(vec.x / magnitude, vec.y / magnitude);
  return normalized;
}

bool Vec2::operator==(const Vec2 &rhs) const
{
  return x == rhs.x && y == rhs.y;
}

bool Vec2::operator!=(const Vec2 &rhs) const
{
  return x != rhs.x || y != rhs.y;
}

void Vec2::operator+=(const Vec2 &rhs)
{
  x += rhs.x;
  y += rhs.y;
}

void Vec2::operator-=(const Vec2 &rhs)
{
  x -= rhs.x;
  y -= rhs.y;
}

void Vec2::operator*=(const float val)
{
  x *= val;
  y *= val;
}

void Vec2::operator/=(const float val)
{
  x /= val;
  y /= val;
}

Vec2 Vec2::dist(const Vec2 &rhs) const
{
  float dx = rhs.x - x;
  float dy = rhs.y - y;
  return Vec2(dx, dy);
}
