#include "window.hpp"

using namespace GearsEngine;

SDL_Window *Window::getWindowHandle() { return window; }
SDL_Event  *Window::getWindowEvent()  { return &event; }

void
Window::isCreated( bool new_switch ) { is_created = new_switch; }

bool
Window::isCreated(void) { return is_created; }

void
Window::isClosed( bool new_switch ) { is_to_be_closed = new_switch; }

bool
Window::isClosed(void) { return is_to_be_closed; }

void
Window::isTitleSet( bool new_switch ) { title_set = new_switch; }

void
Window::isDimensionsSet( bool new_switch ) { dimensions_set = new_switch; }

void
Window::isFlagsSet( bool new_switch ) { flags_set = new_switch; }

bool
Window::isTitleSet(void) { return title_set; }

bool
Window::isDimensionsSet(void) { return dimensions_set; }

bool
Window::isFlagsSet(void) { return flags_set; }

void
Window::isHardwareCapable( bool new_switch )
{ is_hardware_capable = new_switch; }

bool
Window::isHardwareCapable(void) { return is_hardware_capable; }

void
Window::setTitle( const char *new_title )
{
    title = new_title;
    isTitleSet( true );
}

void
Window::setDimensions( int new_width, int new_height )
{
    width = new_width;
    height = new_height;

    isDimensionsSet( true );
}

void
Window::setFlags( Uint32 new_flags )
{
    flags = new_flags;
    isFlagsSet( true );
}

void
Window::setWidth( int new_width ) { width = new_width; }

void
Window::setHeight( int new_height ) { height = new_height; }

int
Window::getWidth() { return width; }

int
Window::getHeight() { return height; }

void
Window::create()
{
    if (title_set && dimensions_set && flags_set) {
        if (SDL_WasInit(SDL_INIT_VIDEO) == 0)
            SDL_Init( SDL_INIT_VIDEO );
        if (SDL_WasInit(SDL_INIT_EVENTS) == 0)
            SDL_Init( SDL_INIT_EVENTS );

        window = SDL_CreateWindow(
                title,
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                width, height,
                flags
        );

        SDL_GetWindowSize(
                window,
                &this->width,
                &this->height
        );

        isHardwareCapable( true );

        if (isHardwareCapable()) {
            SDL_GL_CreateContext( getWindowHandle() );
            SDL_GL_SetSwapInterval( 0 ); // don't wait for vertical-sync
        }
    } else {
        // TODO: report an error of some sorts...
    }
}

void
Window::close() { isClosed( true ); }

void
Window::addAction( Uint32 trigger, Action callback, Data userdata )
{
    actions[trigger] = callback;
    usrdata[trigger] = userdata;
}

void
Window::addKeyboardAction( SDL_Keycode trigger, KeyMode mode, Action callback, Data userdata )
{
    key_actions[trigger][mode] = callback;
    key_usrdata[trigger][mode] = userdata;
}

void
Window::invokeAction( SDL_Event event )
{
    if (actions.find(event.type) != actions.end()) {
        actions[event.type](usrdata[event.type]);
    }
}

void
Window::setKeyState( SDL_KeyboardEvent event )
{
    key_states[event.keysym.sym] = event.state;
}

bool
Window::isPressed( SDL_Keycode code )
{
    return (key_states[code] == SDL_PRESSED);
}

bool
Window::isReleased( SDL_Keycode code )
{
    return (key_states[code] == SDL_RELEASED);
}

void
Window::invokeKeyboardAction( SDL_Keycode trigger, KeyMode mode )
{
    if (key_actions.find(trigger) != key_actions.end())
        if (key_actions[trigger].find(mode) != key_actions[trigger].end())
            key_actions[trigger][mode](key_usrdata[trigger][mode]);
}

RenderWindow::RenderWindow()
{
    isTitleSet( false );
    isDimensionsSet( false );
    isClosed( false );

    setFlags( SDL_WINDOW_OPENGL );

    isHardwareCapable( true );
}

void
RenderWindow::update()
{
    while (SDL_PollEvent(getWindowEvent())) {
        if (getWindowEvent()->type == SDL_KEYDOWN)
            invokeKeyboardAction( getWindowEvent()->key.keysym.sym, KEY_DOWN );
        else if (getWindowEvent()->type == SDL_KEYUP)
            invokeKeyboardAction( getWindowEvent()->key.keysym.sym, KEY_UP );
        else invokeAction( *getWindowEvent() );
    }

    SDL_GL_SwapWindow( getWindowHandle() );
}

/*
///====== OLD ======///
Window::Window( const char *title, Uint width, Uint height, Uint32 flags )
{
    isInitialized( false );
    isHardwareAccelerated( false );

    if (SDL_WasInit(SDL_INIT_VIDEO) == 0)
        SDL_Init( SDL_INIT_VIDEO );
    if (SDL_WasInit(SDL_INIT_EVENTS) == 0)
        SDL_Init( SDL_INIT_EVENTS );

    window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width, height,
            flags
    );

    SDL_GetWindowSize(
            window,
            &this->width,
            &this->height
    );

    if (window != NULL) {
        isInitialized( true );
        isOpen( true );
        if ((flags&SDL_WINDOW_OPENGL) == SDL_WINDOW_OPENGL)
            isHardwareAccelerated( true );
    } // TODO: report an error if else...
}

Window::~Window()
{
    SDL_DestroyWindow( window );
}

int
Window::getWidth() { return width; }

int
Window::getHeight() { return height; }

void
Window::close()
{
    isOpen( false );
}

void
Window::addAction(
        Uint32 trigger,
        void (*action)(Data),
        void *userdata)
{
    actions[trigger] = action;
    usrdata[trigger] = userdata;
}

void
Window::invokeAction( SDL_Event event )
{
    if (actions.find(event.type) != actions.end()) {
        actions[event.type](usrdata[event.type]);
    }
}
    
SDL_Window
*Window::getWindowHandle() { return window; }

bool
Window::isOpen() { return is_open; }

bool
Window::isInitialized() { return is_initialized; }

bool
Window::isHardwareAccelerated() { return is_gl_capable; }

void
Window::isOpen( bool new_value ) { is_open = new_value; }

void
Window::isInitialized( bool new_value ) { is_initialized = new_value; }

void
Window::isHardwareAccelerated( bool new_value ) { is_gl_capable = new_value; }

RenderWindow::RenderWindow( const char *title, Uint width, Uint height )
{
    if (SDL_WasInit(SDL_INIT_VIDEO) == 0)
        SDL_Init( SDL_INIT_VIDEO );
    if (SDL_WasInit(SDL_INIT_EVENTS) == 0)
        SDL_Init( SDL_INIT_EVENTS );

    window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width, height,
            flags
    );

    SDL_GetWindowSize(
            window,
            &this->width,
            &this->height
    );

    if (window != NULL) {
        isInitialized( true );
        isOpen( true );
        if ((flags&SDL_WINDOW_OPENGL) == SDL_WINDOW_OPENGL)
            isHardwareAccelerated( true );
    } // TODO: report an error if else...

    if (isHardwareAccelerated()) {
        context = SDL_GL_CreateContext( getWindowHandle() );
        SDL_GL_MakeCurrent( getWindowHandle(), context );
    } // TODO: report an error if else...
}

void
RenderWindow::update()
{
    while (SDL_PollEvent(getWindowEvent())) invokeAction( *getWindowEvent() );
    SDL_GL_SwapWindow( getWindowHandle() );
}
*/
