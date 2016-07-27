#ifndef _GEARS_WINDOW_HPP_
#define _GEARS_WINDOW_HPP_

#include <SDL2/SDL.h>
#include <map>

typedef enum {
    KEY_DOWN,
    KEY_UP
} KeyMode;

typedef void* Data;
typedef void  (*Action)(Data);

typedef std::map<Uint32, Action> ActionMap;
typedef std::map<Uint32, Data>   DataMap;

typedef std::map<KeyMode, Action> KMAMap;
typedef std::map<KeyMode, Data>   KMDMap;

typedef std::map<SDL_Keycode, KMAMap> KeyActionMap;
typedef std::map<SDL_Keycode, KMDMap> KeyDataMap;

typedef unsigned int Uint;

namespace GearsEngine {
    class Window {
        private:
            ActionMap actions;
            DataMap   usrdata;

            KeyActionMap key_actions;
            KeyDataMap   key_usrdata;

            std::map<SDL_Keycode, Uint8> key_states;

        public:
            SDL_Window *getWindowHandle();
            SDL_Event  *getWindowEvent();

            void setKeyState( SDL_KeyboardEvent event );
            bool isPressed( SDL_Keycode code );
            bool isReleased( SDL_Keycode code );

            void isCreated( bool new_switch );
            bool isCreated(void);

            void isClosed( bool new_switch );
            bool isClosed(void);

            void isTitleSet( bool new_switch );
            void isDimensionsSet( bool new_switch );
            void isFlagsSet( bool new_switch );

            bool isTitleSet(void);
            bool isDimensionsSet(void);
            bool isFlagsSet(void);

            void isHardwareCapable( bool new_switch );
            bool isHardwareCapable();

            //===========
            //set various parameters before the window is created.
            //===========
            void setTitle( const char *new_title );
            void setDimensions( int new_width, int new_height );
            void setFlags( Uint32 new_flags );

            //===========
            //[set/get]Width  : modify/return current width parameter.
            //[set/get]Height : modify/return current height parameter.
            //===========
            void setWidth( int new_width );
            void setHeight( int new_height );
            //-----------
            int getWidth();
            int getHeight();

            //===========
            //create the window
            //===========
            void create();

            //===========
            //closes the window
            //===========
            void close();

            void addAction( Uint32 trigger, Action callback, Data userdata );
            void addKeyboardAction( SDL_Keycode trigger, KeyMode mode, Action callback, Data userdata );
            void invokeAction( SDL_Event event );
            void invokeKeyboardAction( SDL_Keycode trigger, KeyMode mode );

            //===========
            //update the window's current state
            //===========
            virtual void update() = 0; // is a pure-virtual void


        private:
            SDL_Window *window;
            SDL_Event   event;

            const char *title; // the window's current title.
            int width, height;
            Uint32 flags;

            bool is_to_be_closed;

            // various attribute-checks that need to be true before the window
            // can be rendered...
            bool title_set,
                 dimensions_set,
                 flags_set;

            // is capable of hardware-accelerated rendering.
            bool is_hardware_capable;

            bool is_created;
    };

    class RenderWindow : public Window {
        public:
            RenderWindow();
            void update();

        private:
            SDL_GLContext *context;
    };
}

#endif // _GEARS_WINDOW_HPP_
