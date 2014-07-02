#ifndef VECTOR2D_H_
#define VECTOR2D_H_

template <typename T>
struct Vector2D {
   Vector2D() {}
   Vector2D(T v) : x(v), y(v) {}
   Vector2D(T x, T y) : x(x), y(y) {}

   Vector2D operator+(const Vector2D& o) const {
      return Vector2D(x + o.x, y + o.y);
   }

   Vector2D operator-(const Vector2D& o) const {
      return operator+(o * -1);
   }

   Vector2D operator*(T t) const {
      return Vector2D(x * t, y * t);
   }

   Vector2D operator/(T t) const {
      return operator*(1 / t);
   }

   template <typename U>
   Vector2D<U> convert(U (*conversion_func)(T)) {
      return Vector2D<U>(
         conversion_func(x),
         conversion_func(y));
   }

   T x, y;
};

template <typename T>
inline Vector2D<T> operator*(T t, const Vector2D<T>& v) { return v * t; }

#endif // VECTOR2D_H_
