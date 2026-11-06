#include "Matrix.hpp"
#include "Object.hpp"
#include "Vector.hpp"

class Head : public Object
{
  private:
    Head(void);

  public:
    ~Head(void);

    Head(vec3 scv, vec4 clv, const Object& mother);

    void Update(void);
};
