#include <boost/chrono/include.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.hpp"
#include "renderer.hpp"

#define degreesToRadians(x) x*(3.141592f/180.0f)
#define sqr(x) pow(x, 2)

using namespace GearsEngine;

typedef boost::chrono::high_resolution_clock Clock;

typedef boost::chrono::duration<double> Seconds;
typedef boost::chrono::nanoseconds Nanoseconds;

const double dPS = 25.0f;
const double mPS = 5.0f;

glm::vec3 normalize( glm::vec3 input )
{
    double norm = sqrt( sqr(input.x) + sqr(input.y) + sqr(input.z) );

    if (norm > 0) {
        input.x /= norm;
        input.y /= norm;
        input.z /= norm;
    
        return input;
    } else return glm::vec3();
}

class Object {
    private:
        bool is_moving_forward;
        bool is_moving_backward;
        bool is_moving_left;
        bool is_moving_right;

        bool is_rotating_up;
        bool is_rotating_down;
        bool is_rotating_left;
        bool is_rotating_right;

        glm::vec3 position;
        glm::vec3 translate;
        glm::vec3 rotate;

    public:
        bool isRotatingLeft() { return is_rotating_left; }
        void isRotatingLeft( bool b ) { is_rotating_left = b; }

        bool isRotatingRight() { return is_rotating_right; }
        void isRotatingRight( bool b ) { is_rotating_right = b; }

        bool isRotatingUp() { return is_rotating_up; }
        void isRotatingUp( bool b ) { is_rotating_up = b; }

        bool isRotatingDown() { return is_rotating_down; }
        void isRotatingDown( bool b ) { is_rotating_down = b; }

        bool isMovingForward() { return is_moving_forward; }
        void isMovingForward( bool b ) { is_moving_forward = b; }

        bool isMovingBackward() { return is_moving_backward; }
        void isMovingBackward( bool b ) { is_moving_backward = b; }

        bool isMovingLeft() { return is_moving_left; }
        void isMovingLeft( bool b ) { is_moving_left = b; }

        bool isMovingRight() { return is_moving_right; }
        void isMovingRight( bool b ) { is_moving_right = b; }

        double xPosition() { return position.x; }
        double yPosition() { return position.y; }
        double zPosition() { return position.z; }

        double xOrientation() { return rotate.x; }
        double yOrientation() { return rotate.y; }
        double zOrientation() { return rotate.z; }

        void update( Nanoseconds time )
        {
            float dt =
                boost::chrono::duration_cast<Seconds>
                (time).count();

            if (isRotatingLeft()) {
                rotate.y += degreesToRadians( dPS * dt );
            }

            if (isRotatingRight()) {
                rotate.y -= degreesToRadians( dPS * dt );
            }

            if (isRotatingUp()) {
                rotate.x += degreesToRadians( dPS * dt );
            }

            if (isRotatingDown()) {
                rotate.x -= degreesToRadians( dPS * dt );
            }

            if (isMovingForward()) {
                double magnitude = 0.0f;

                translate.y += sin( rotate.x );
                magnitude = cos( rotate.x );

                translate.x += -1.0f * (magnitude * sin( rotate.y ));
                translate.z += magnitude * cos( rotate.y );
            }

            if (isMovingBackward()) {
                double magnitude = 0.0f;

                translate.y += -1.0f * sin( rotate.x );
                magnitude = cos( rotate.x );

                translate.x += (magnitude * sin( rotate.y ));
                translate.z += -1.0f * (magnitude * cos( rotate.y ));
            }
            
            if (isMovingLeft()) {
                translate.x += -1.0f * cos( rotate.y );
                translate.z += -1.0f * sin( rotate.y );
            }

            if (isMovingRight()) {
                translate.x += cos( rotate.y );
                translate.z += sin( rotate.y );
            }

            glm::vec3 normal = normalize( translate );
            
            position.x += normal.x * (mPS * dt);
            position.y += normal.y * (mPS * dt);
            position.z += normal.z * (mPS * dt);

            translate = glm::vec3();
        }
                        
        Object()
        {
            position.x =  0.0f;
            position.y =  0.0f;
            position.z = -10.f;

            translate.x =  0.0f;
            translate.y =  0.0f;
            translate.z =  0.0f;

            rotate.x = 0.0f;
            rotate.y = 0.0f;
            rotate.z = 0.0f;

            isRotatingLeft( false );
            isRotatingRight( false );
            isRotatingUp( false );
            isRotatingDown( false );
            isMovingForward( false );
            isMovingBackward( false );
            isMovingLeft( false );
            isMovingRight( false );
        }
};

class Camera : public Object {};

void move_forward( void* data )
{
    Camera *camera = static_cast<Camera*>(data);
    camera->isMovingForward( true );
}

void stop_move_forward( void* data )
{
    Camera *camera = static_cast<Camera*>(data);
    camera->isMovingForward( false );
}

void move_backward( void* data )
{
    Camera *camera = static_cast<Camera*>(data);
    camera->isMovingBackward( true );
}

void stop_move_backward( void* data )
{
    Camera *camera = static_cast<Camera*>(data);
    camera->isMovingBackward( false );
}

void move_left( void* data )
{
    Camera *camera = static_cast<Camera*>(data);
    camera->isMovingLeft( true );
}

void stop_move_left( void* data )
{
    Camera *camera = static_cast<Camera*>(data);
    camera->isMovingLeft( false );
}

void move_right( void* data )
{
    Camera *camera = static_cast<Camera*>(data);
    camera->isMovingRight( true );
}

void stop_move_right( void* data )
{
    Camera *camera = static_cast<Camera*>(data);
    camera->isMovingRight( false );
}

void rotate_left( void* data )
{
    Camera *camera = static_cast<Camera*>(data);
    camera->isRotatingLeft( true );
}

void stop_rotate_left( void* data )
{
    Camera *camera = static_cast<Camera*>(data);
    camera->isRotatingLeft( false );
}

void rotate_right( void* data )
{
    Camera *camera = static_cast<Camera*>(data);
    camera->isRotatingRight( true );
}

void stop_rotate_right( void* data )
{
    Camera *camera = static_cast<Camera*>(data);
    camera->isRotatingRight( false );
}

void rotate_up( void* data )
{
    Camera *camera = static_cast<Camera*>(data);
    camera->isRotatingUp( true );
}

void stop_rotate_up( void* data )
{
    Camera *camera = static_cast<Camera*>(data);
    camera->isRotatingUp( false );
}

void rotate_down( void* data )
{
    Camera *camera = static_cast<Camera*>(data);
    camera->isRotatingDown( true );
}

void stop_rotate_down( void* data )
{
    Camera *camera = static_cast<Camera*>(data);
    camera->isRotatingDown( false );
}

void close_window( void* window )
{
    if (window != NULL) {
        Window *w = static_cast<Window*>(window);
        w->close();
    }
}

GLfloat gayLERP5( GLfloat axis )
{
    return (-0.79f*pow(axis, 2)) + 20.0f;
}

int main( int argc, char *argv[] )
{
    std::ofstream out("out.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

    Clock clock;
    Nanoseconds lastTime;
    Nanoseconds accumulator;
    Nanoseconds timer;

    lastTime -= lastTime;
    accumulator -= accumulator;

    Window *window = new RenderWindow();

    window->setTitle( "Window test." );
    window->setDimensions( 640, 480 );
    window->create();

    Renderer renderer( window );

    GLfloat vertices[] = {
        -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  // Top Right
         1.0f, -1.0f,  1.0f,  0.5f, 0.5f, 0.0f,  // Bottom Right
         1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,  // Bottom Left
        -1.0f,  1.0f,  1.0f,  0.0f, 0.5f, 0.5f,  // Top Left

        -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  // Top Right
         1.0f, -1.0f, -1.0f,  0.5f, 0.5f, 0.0f,  // Bottom Right
         1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  // Bottom Left
        -1.0f,  1.0f, -1.0f,  0.0f, 0.5f, 0.5f,  // Top Left
    };

    GLuint indices[] = {
        // front
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3, 
    };

    const GLchar *vertexSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec3 color;\n"
        "out vec3 outColor;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * view * model * vec4( position.x, position.y, position.z, 1.0 );\n"
        "   outColor = color;\n"
        "}\n\0"
    ;

    const GLchar *fragmentSource =
        "#version 330 core\n"
        "in vec3 outColor;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = vec4( outColor, 1.0 );\n"
        "}\n\0"
    ;

    VAPconfig configuration0;
    configuration0.index = 0;
    configuration0.size = 3;
    configuration0.type = GL_FLOAT;
    configuration0.normalized = GL_FALSE;
    configuration0.stride = 6 * sizeof(GLfloat);
    configuration0.pointer = (GLvoid*)0;

    VAPconfig configuration1;
    configuration1.index = 1;
    configuration1.size = 3;
    configuration1.type = GL_FLOAT;
    configuration1.normalized = GL_FALSE;
    configuration1.stride = 6 * sizeof(GLfloat);
    configuration1.pointer = (GLvoid*)(3 * sizeof(GLfloat));

    renderer.addVAPConfiguration( configuration0 );
    renderer.addVAPConfiguration( configuration1 );
    int vapIndex = renderer.linkVAPModule();

    VertexBuffer vbo = renderer.generateVBO( "VBO_FIRST" );
    ElementBuffer ebo = renderer.generateEBO( "EBO_FIRST" );
    vbo.data = vertices;
    vbo.size = sizeof(vertices);
    ebo.data = indices;
    ebo.size = sizeof(indices);
    renderer.initializeVertexBuffer( vbo, ebo, vapIndex );

    VertexShader vShader;
    FragmentShader fShader;

    vShader.source_code = vertexSource;
    fShader.source_code = fragmentSource;

    ShaderProgram shaderProgram =
        renderer.createShaderProgram( "SHADER_FIRST", &vShader, &fShader );

    GLuint modelL =
        glGetUniformLocation( shaderProgram.uid, "model" );
    GLuint viewL =
        glGetUniformLocation( shaderProgram.uid, "view" );
    GLuint projectionL = 
        glGetUniformLocation( shaderProgram.uid, "projection" );

    GLfloat xDegree = 0.0f;
    GLfloat yDegree = 0.0f;

    Camera camera;

    window->addAction( SDL_QUIT, &close_window, static_cast<void*>(window) );
    window->addKeyboardAction( SDLK_ESCAPE, KEY_UP, &close_window, static_cast<void*>(window) );
    window->addKeyboardAction( SDLK_w, KEY_DOWN, &move_forward, static_cast<void*>(&camera) );
    window->addKeyboardAction( SDLK_w, KEY_UP, &stop_move_forward, static_cast<void*>(&camera) );
    window->addKeyboardAction( SDLK_s, KEY_DOWN, &move_backward, static_cast<void*>(&camera) );
    window->addKeyboardAction( SDLK_s, KEY_UP, &stop_move_backward, static_cast<void*>(&camera) );
    window->addKeyboardAction( SDLK_a, KEY_DOWN, &move_left, static_cast<void*>(&camera) );
    window->addKeyboardAction( SDLK_a, KEY_UP, &stop_move_left, static_cast<void*>(&camera) );
    window->addKeyboardAction( SDLK_d, KEY_DOWN, &move_right, static_cast<void*>(&camera) );
    window->addKeyboardAction( SDLK_d, KEY_UP, &stop_move_right, static_cast<void*>(&camera) );
    window->addKeyboardAction( SDLK_q, KEY_DOWN, &rotate_left, static_cast<void*>(&camera) );
    window->addKeyboardAction( SDLK_q, KEY_UP, &stop_rotate_left, static_cast<void*>(&camera) );
    window->addKeyboardAction( SDLK_e, KEY_DOWN, &rotate_right, static_cast<void*>(&camera) );
    window->addKeyboardAction( SDLK_e, KEY_UP, &stop_rotate_right, static_cast<void*>(&camera) );
    window->addKeyboardAction( SDLK_z, KEY_DOWN, &rotate_down, static_cast<void*>(&camera) );
    window->addKeyboardAction( SDLK_z, KEY_UP, &stop_rotate_down, static_cast<void*>(&camera) );
    window->addKeyboardAction( SDLK_c, KEY_DOWN, &rotate_up, static_cast<void*>(&camera) );
    window->addKeyboardAction( SDLK_c, KEY_UP, &stop_rotate_up, static_cast<void*>(&camera) );

    glm::mat4 model, view, projection;

    lastTime = clock.now().time_since_epoch();

    glm::vec3 cubes[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f), 
      glm::vec3( 2.0f,  5.0f, -15.0f), 
      glm::vec3(-1.5f, -2.2f, -2.5f),  
      glm::vec3(-3.8f, -2.0f, -12.3f),  
      glm::vec3( 2.4f, -0.4f, -3.5f),  
      glm::vec3(-1.7f,  3.0f, -7.5f),  
      glm::vec3( 5.0f,  2.5f, -5.0f),
      glm::vec3( 1.3f, -2.0f, -2.5f),  
      glm::vec3( 1.5f,  2.0f, -2.5f), 
      glm::vec3( 1.5f,  0.2f, -1.5f), 
      glm::vec3( 9.0f,  3.2f, -1.0f),
      glm::vec3(-1.0f,  3.2f,  8.0f),
      glm::vec3( 7.0f,  3.0f, -2.0f),
      glm::vec3( 6.0f,  5.2f, -2.6f),
      glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    GLfloat cubeDegree[] = {
        0,
        25.3f,
        30.5f,
        94.3f,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0
    };

    timer -= timer;
    long long int frames = 0;
    bool goingUp = true;

    GLfloat cameraY = 0.0f;
    GLfloat cameraRot = 0.0f;

    int delay_time = 1;
    do {
        accumulator = clock.now().time_since_epoch() - lastTime;
        lastTime = clock.now().time_since_epoch();
        timer += accumulator;

        if (timer.count() > 1000000000) {
            std::cout << "frames this second: " << frames << '\n';
            frames = 0;
            timer -= Nanoseconds(1000000000);
        }

        camera.update( accumulator );

        renderer.clear( 0.0, 1.0, 1.0, 1.0 );

        for (GLuint i = 0; i < sizeof(cubes)/sizeof(glm::vec3); i++) {
            model =
                glm::translate(
                        model,
                        cubes[i] );

            cubeDegree[i] += static_cast<GLfloat>(10.0f*i*(accumulator.count()/1000000000.0f));

            model =
                glm::rotate( 
                        model,
                        degreesToRadians(cubeDegree[i]),
                        glm::vec3(0.0f, 1.0f, 0.0f) );

            model =
                glm::rotate(
                        model,
                        degreesToRadians(xDegree+(25*i)),
                        glm::vec3(1.0f, 0.0f, 0.0f)
                );

            model =
                glm::scale(
                        model,
                        glm::vec3( 0.5f, 0.5f, 0.5f )
                );

            view =
                glm::rotate(
                        view,
                        degreesToRadians(0.0f),
                        glm::vec3(1.0f, 0.0f, 0.0f) );

            view  = glm::rotate( view, (GLfloat)camera.xOrientation(), glm::vec3(1.0f, 0.0f, 0.0f) );
            view  = glm::rotate( view, (GLfloat)camera.yOrientation(), glm::vec3( 0.0f, 1.0f, 0.0f ) );
            view  = glm::translate( view, glm::vec3(camera.xPosition(), camera.yPosition(), camera.zPosition()) );

            projection = glm::perspective( -45.0f, 640.0f/480.0f, 0.1f, 100.0f );

            glUniformMatrix4fv( modelL, 1, GL_FALSE, glm::value_ptr(model) );
            glUniformMatrix4fv( viewL, 1, GL_FALSE, glm::value_ptr(view) );
            glUniformMatrix4fv( projectionL, 1, GL_FALSE, glm::value_ptr(projection) );

            renderer.draw( GR_RENDER_ELEMENTS );

            model = glm::mat4();
            view = glm::mat4();
            projection = glm::mat4();
        }
        
        window->update();
        frames++;
    } while (!window->isClosed());
    
    return 0;
}
