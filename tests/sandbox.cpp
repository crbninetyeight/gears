#include "window.hpp"
#include "renderer.hpp"

using namespace GearsEngine;

int main( int argc, char **argv )
{
    Window *window = new RenderWindow();

    window->setTitle( "Sandbox" );
    window->setDimensions( 640, 480 );
    window->create();

    Renderer *renderer = new Renderer( window );

    delete renderer;
    delete window;

    return 0;
}
