#include "Matrix.hpp"
#include "Object.hpp"
#include "Vector.hpp"

class Torso : public Object
{
  private:
    Torso(void);

  public:
    ~Torso(void);

    Torso(vec3 scv, vec4 clv, mat4 rMod);

    void Update(void);
};
