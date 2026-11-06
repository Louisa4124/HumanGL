#include "Matrix.hpp"
#include "Object.hpp"
#include "Vector.hpp"
#include "internal.hpp"

class ForeArm : public Object
{
  private:
    ForeArm(void);

    e_side side;

  public:
    ~ForeArm(void);

    ForeArm(vec3 scv, vec4 clv, e_side side, const Object& mother);

    void Update(void);
};
