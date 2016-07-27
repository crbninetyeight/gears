#include "renderer.hpp"
#include <cassert>
#include <iostream>

using namespace GearsEngine;

Renderer::Renderer( Window *target )
{
    current_active_vao = 0;
    current_active_shader = 0;

    if (target != NULL && target->isHardwareCapable()) {

        glewExperimental = true;
        GLenum error = glewInit();

        if (GLEW_OK != error) {
            // TODO: produce an error of some sorts...
        }

        glViewport( 0, 0, target->getWidth(), target->getHeight() );
        setActiveVertexArray( generateVAO("VAO_DEFAULT") );
    }
}

int
Renderer::linkVAPModule()
{
    vap_modules.push_back( vap_configurations );
    vap_configurations.clear();

    return vap_modules.size() - 1;
}

void
Renderer::addVAPConfiguration( VAPconfig new_configuration )
{
    vap_configurations.push_back( new_configuration );
}

VAPconfig
Renderer::getVAPConfiguration( unsigned int index )
{
    return vap_configurations[index];
}

VertexArray
Renderer::generateVAO( std::string identifier )
{
    VertexArray vao;

    glGenVertexArrays( 1, &vao.uid );
    assert( glGetError() == GL_NO_ERROR );
    vao.identifier = identifier;
    vertex_arrays[identifier] = vao;

    return vao;
}

VertexBuffer
Renderer::generateVBO( std::string identifier )
{
    VertexBuffer vbo;

    glGenBuffers( 1, &vbo.uid );
    assert( glGetError() == GL_NO_ERROR );
    vbo.identifier = identifier;
    vertex_buffers[identifier] = vbo;

    return vbo;
}

ElementBuffer
Renderer::generateEBO( std::string identifier )
{
    ElementBuffer ebo;

    glGenBuffers( 1, &ebo.uid );
    assert( glGetError() == GL_NO_ERROR );
    ebo.identifier = identifier;
    element_buffers[identifier] = ebo;

    return ebo;
}

VertexArray
Renderer::getVAO( std::string identifier )
{
    if (vertex_arrays.find(identifier) != vertex_arrays.end())
        return vertex_arrays[identifier];
    else {
        VertexArray dummy;
        dummy.uid = 0;
        return dummy;
    }
}

VertexBuffer
Renderer::getVBO( std::string identifier )
{
    if (vertex_buffers.find(identifier) != vertex_buffers.end())
        return vertex_buffers[identifier];
    else {
        VertexBuffer dummy;
        dummy.uid = 0;
        return dummy;
    }
}

ElementBuffer
Renderer::getEBO( std::string identifier )
{
    if (element_buffers.find(identifier) != element_buffers.end())
        return element_buffers[identifier];
    else {
        ElementBuffer dummy;
        dummy.uid = 0;
        return dummy;
    }
}

ShaderProgram
Renderer::createShaderProgram(
        std::string identifier,
        VertexShader *vertex_shader,
        FragmentShader *fragment_shader )
{
    ShaderProgram shader_program;
    shader_program.uid = 0;
    shader_program.identifier = identifier;

    vertex_shader->uid = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource(
            vertex_shader->uid,
            1, &vertex_shader->source_code,
            NULL
    );

    glCompileShader( vertex_shader->uid );
    
    fragment_shader->uid = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource(
            fragment_shader->uid,
            1, &fragment_shader->source_code,
            NULL
    );

    glCompileShader( fragment_shader->uid );

    shader_program.uid = glCreateProgram();
    glAttachShader( shader_program.uid, vertex_shader->uid );
    glAttachShader( shader_program.uid, fragment_shader->uid );

    glLinkProgram( shader_program.uid );

    shader_program.vertex_shader = *vertex_shader;
    shader_program.fragment_shader = *fragment_shader;

    shader_programs[identifier] = shader_program;

    current_active_shader = shader_program.uid;

    return shader_program;
}

void
Renderer::setUniform( GLuint v0, GLint location )
{
    glUniform1ui( location, v0 );
}

void
Renderer::setUniform( GLfloat v0, GLint location )
{
    glUniform1f( location, v0 );
}

void
Renderer::setUniform(
        const GLfloat *v0,
        GLsizei count,
        GLboolean transpose,
        GLuint location )
{
    glUniformMatrix4fv( location, count, transpose, v0 );
}

GLint
Renderer::getUniform( std::string uniform, ShaderProgram program )
{
    return glGetUniformLocation( program.uid, uniform.c_str() );
}


void
Renderer::setActiveVertexArray( VertexArray vao )
{
    current_active_vao = vao.uid;
}

void
Renderer::initializeVertexBuffer( 
        VertexBuffer vbo,
        ElementBuffer ebo,
        int vap )
{
    glBindVertexArray( current_active_vao );
    vertex_buffers[vbo.identifier] = vbo;

    glBindBuffer( GL_ARRAY_BUFFER, vbo.uid );

    glBufferData(
            GL_ARRAY_BUFFER,
            vbo.size,
            vbo.data,
            GL_STATIC_DRAW
    );

    if (ebo.uid > 0) {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo.uid );
        glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                ebo.size,
                ebo.data,
                GL_STATIC_DRAW
        );
    }

    for (int c = 0; c < vap_modules[vap].size(); ++c ) {
        glVertexAttribPointer(
                vap_modules[vap][c].index,
                vap_modules[vap][c].size,
                vap_modules[vap][c].type,
                vap_modules[vap][c].normalized,
                vap_modules[vap][c].stride,
                vap_modules[vap][c].pointer
        );

        glEnableVertexAttribArray( vap_modules[vap][c].index );
    }

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}

void
Renderer::initializeElementBuffer( ElementBuffer ebo )
{
    glBindVertexArray( current_active_vao );
    element_buffers[ebo.identifier] = ebo;
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo.uid );

    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            ebo.size,
            ebo.data,
            GL_STATIC_DRAW
    );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

    current_active_ebo = ebo.uid;
}

void
Renderer::setActiveShaderProgram( ShaderProgram shader_program )
{
    current_active_shader = shader_program.uid;
}

void
Renderer::clear( GLclampf r, GLclampf g, GLclampf b, GLclampf a )
{
    glClearColor( r, g, b, a );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void
Renderer::draw( RenderType mode )
{
    glUseProgram( current_active_shader );
    glBindVertexArray( current_active_vao );

    glEnable( GL_DEPTH_TEST );
    glDrawElements( GL_TRIANGLES, 12*3, GL_UNSIGNED_INT, 0 );

    glBindVertexArray( 0 );
}
