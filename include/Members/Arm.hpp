#include "Matrix.hpp"
#include "Object.hpp"
#include "Vector.hpp"
#include "internal.hpp"

class Arm : public Object
{
  private:
    Arm(void);
    e_side side;


  public:
    ~Arm(void);

    Arm(vec3 scv, vec4 clv, e_side side, const Object& mother);

    void Update(void);
};
