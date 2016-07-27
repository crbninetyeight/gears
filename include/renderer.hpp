#ifndef _APO_RENDERER_HPP_
#define _APO_RENDERER_HPP_

#include <GL/glew.h>
#include <string>
#include <map>
#include <vector>
#include "window.hpp"

typedef enum {
    GR_RENDER_ELEMENTS = 0,
    GR_RENDER_ARRAYS
} RenderType;

typedef struct {
    GLuint uid;
    std::string identifier;

    const GLchar *source_code;
} VertexShader, FragmentShader;

typedef struct {
    GLuint uid;
    std::string identifier;

    VertexShader vertex_shader;
    FragmentShader fragment_shader;
} ShaderProgram;

typedef struct {
    GLuint uid;
    std::string identifier;
} VertexArray;

typedef struct {
    GLuint uid;
    std::string identifier;

    GLuint size;
    GLuint attrib_count;

    GLvoid *data;
} VertexBuffer, ElementBuffer;

typedef struct {
    GLuint index;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    const GLvoid *pointer;
} VAPconfig;

typedef std::map<std::string, VertexArray>   VAOMap;
typedef std::map<std::string, VertexBuffer>  VBOMap;
typedef std::map<std::string, ElementBuffer> EBOMap;
typedef std::map<std::string, ShaderProgram> PGMMap;

typedef std::vector<VAPconfig> VAPMap;
typedef std::vector<VAPMap>    VAPMod;

namespace GearsEngine {
    class Renderer
    {
        private:
            GLuint current_active_vao;
            GLuint current_active_ebo;

            GLuint current_active_shader;

            VAPMod vap_modules;
            VAPMap vap_configurations;

            VAOMap vertex_arrays;
            VBOMap vertex_buffers;
            EBOMap element_buffers;
            PGMMap shader_programs;

        public:
            Renderer( Window *window );

            void addVAPConfiguration( VAPconfig new_configuration );
            int  linkVAPModule();

            VAPconfig getVAPConfiguration( unsigned int index );

            VertexArray   generateVAO( std::string identifier );
            VertexBuffer  generateVBO( std::string identifier );
            ElementBuffer generateEBO( std::string identifier );

            VertexArray   getVAO( std::string identifier );
            VertexBuffer  getVBO( std::string identifier );
            ElementBuffer getEBO( std::string identifier );

            ShaderProgram createShaderProgram(
                    std::string identifier,
                    VertexShader *vertex_shader,
                    FragmentShader *fragment_shader
            );

            void  setUniform( GLuint v0, GLint location );
            void  setUniform( GLfloat v0, GLint location );
            void  setUniform( const GLfloat *v0, GLsizei count, GLboolean transpose, GLuint location );
            GLint getUniform( std::string uniform, ShaderProgram program );

            void setActiveVertexArray( VertexArray vao );

            void initializeVertexBuffer( 
                    VertexBuffer vbo,
                    ElementBuffer ebo,
                    int vap
            );

            void initializeElementBuffer( ElementBuffer ebo );

            void setActiveShaderProgram( ShaderProgram shader_program );

            void clear( GLclampf r, GLclampf g, GLclampf b, GLclampf a );
            void draw( RenderType mode );
    };
}

#endif // _APO_RENDERER_HPP_
