#ifndef RECTANGLE_H_
#define RECTANGLE_H_

struct Rectangle {
   Rectangle(int x, int y, int width, int height) :
      x_(x),
      y_(y),
      width_(width),
      height_(height) {}

   int left() const { return x_; }
   int right() const { return x_ + width_; }
   int top() const { return y_; }
   int bottom() const { return y_ + height_; }

   int width() const { return width_; }
   int height() const { return height_; }
  private:
   const int x_, y_, width_, height_;
};

#endif // RECTANGLE_H_