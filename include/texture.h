#include <GL/glew.h>
#include "external/stb_image.h"

namespace GLEngine
{   
    class Texture   //   Stores the texture information 
    {
    public:
        unsigned int ID; 

        Texture(); 
        ~Texture(); 
    };
}