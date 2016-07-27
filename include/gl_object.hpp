#ifndef _GEARS_GL_OBJECT_HPP_
#define _GEARS_GL_OBJECT_HPP_

#include <GL/glew.h>

typedef struct
{
    typedef GLuint value_type;
    static value_type Create();
    static void Destroy(value_type);
} vaoTraits, vboTraits;

template<typename T>
class glObject
{
    public:
        glObject() : m_obj(T::Create()) {}
        ~glObject() {T::Destroy(m_obj);}

        operator typename T::value_type() {return m_obj;}

    private:
        typename T::value_type m_obj;
};

#endif
