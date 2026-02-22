#pragma once

constexpr float  PI_F = 3.1415927f;
constexpr double PI_D = 3.141592653589793;
constexpr float  HALF_PI_F = PI_F / 2.0f;
constexpr double HAFL_PI_D = PI_D / 2.0;

struct FMatrix4x4 { float data[16]; };
struct FPoint3D { float x, y, z; };
struct FPoint2D { float x, y; };
struct IPoint2D { int   x, y; };
