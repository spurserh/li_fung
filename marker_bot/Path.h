
#ifndef PATH_I
#define PATH_I

class PathI {
 public:
  // t is 0 to 1.
  // Returns meters
  virtual
  Vec2f target_position(float t)const = 0;
};

class LinearPath : public PathI {
 public:
  LinearPath(Vec2f const&offset)
   : offset(offset) {
  }
  
  Vec2f target_position(float t)const override {
    return offset * t;
  }
  
 private:
 
  const Vec2f offset; 
};

#endif//PATH_I

