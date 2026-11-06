#include "Matrix.hpp"
#include "Object.hpp"
#include "Vector.hpp"
#include "internal.hpp"

class LowerLeg : public Object
{
  private:
    LowerLeg(void);
    e_side side;

  public:
    ~LowerLeg(void);

    LowerLeg(vec3 scv, vec4 clv, e_side side, const Object& mother);

    void Update(void);
};
