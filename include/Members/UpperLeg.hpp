#include "Matrix.hpp"
#include "Object.hpp"
#include "Vector.hpp"
#include "internal.hpp"

class UpperLeg : public Object
{
  private:
    UpperLeg(void);
    e_side side;

  public:
    ~UpperLeg(void);

    UpperLeg(vec3 scv, vec4 clv, e_side side, const Object& mother);

    void Update(void);
};
