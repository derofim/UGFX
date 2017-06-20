// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#pragma once

#include "exa.h"
#include <map>

namespace exa
{
	class Image;

	struct WindowSize {
		int32 width;
		int32 height;
		float aspect;
	};

	struct WindowLocation {
		int32 x;
		int32 y;
	};

	enum class WindowBorder {
		borderless,
		bordered,
		/** The total count of available window border states */
		TotalCount
	};

	enum class WindowVisibility {
		hidden,
		visible,
		/** The total count of available window visibility states */
		TotalCount
	};

	enum class WindowState {
		minimazed,
		maximized,
		/** The total count of available window states */
		TotalCount
	};

	enum class MouseCursor {

		ARROW,
		CROSSHAIR,
		DEFAULT,
		MOVE,
		POINTER,
		RESIZE_NESW,
		RESIZE_NS,
		RESIZE_NWSE,
		RESIZE_WE,
		TEXT,
		WAIT,
		WAIT_ARROW,
		CUSTOM

	};

	class SDLMouse {
	public:
		static SDL_Cursor* arrowCursor;
		static SDL_Cursor* crosshairCursor;
		static SDL_Cursor* moveCursor;
		static SDL_Cursor* pointerCursor;
		static SDL_Cursor* resizeNESWCursor;
		static SDL_Cursor* resizeNSCursor;
		static SDL_Cursor* resizeNWSECursor;
		static SDL_Cursor* resizeWECursor;
		static SDL_Cursor* textCursor;
		static SDL_Cursor* waitCursor;
		static SDL_Cursor* waitArrowCursor;
	};

	enum class WindowMode {
		/** fullscreen window based on SDL_WINDOW_FULLSCREEN **/
		Fullscreen,
		/** fullscreen window at the current desktop resolution **/
		Windowed,
		/** fullscreen window at the current desktop resolution based on SDL_WINDOW_FULLSCREEN_DESKTOP  **/
		fakeFullscreen,
		/** The total count of available window modes */
		TotalCount
	};

	struct WindowInfo {
		const char* title = "Exagine";
		WindowMode mode = WindowMode::Fullscreen;
		WindowSize size = WindowSize{};
		WindowLocation location = WindowLocation{};
		WindowState trayState = WindowState{};
		WindowVisibility visibility = WindowVisibility::visible;
		WindowBorder border = WindowBorder::borderless;
	};

	class Window
	{
	private:

	public:

		Window();
		~Window();

		bool init();

		// Used to get screen dimensions
		void recalculateDisplayModeRect();

		void setTitle(const char* title);

		const char * getWindowTitle() const;
		
		void recalculateWindowSize();

		// Updates window
		void swapWindow();

		void setSize(int32 width, int32 height);

		void addSize(int32 x, int32 y);

		void setWidth(int32 x);

		int getWindowDisplayIndex();

		void moveMouse(int x, int y);

		void moveMouseGlobal(int x, int y);

		void acceptTextInput(bool enabled = true);

		bool isAcceptingTextInput();

		void setHeight(int32 y);

		void handleReposition(int32 x, int32 y);

		void setPosition(int32 x, int32 y);

		void moveBy(int32 x, int32 y);

		void setX(int32 x);

		void setY(int32 y);

		bool setFullscreen();

		bool recalculateDisplayBounds();

		int setFullscreenMode(Uint32 flags);

		bool isResizable();

		void RestoreDimentions();

		void setIcon(SDL_Surface * surface);

		void setIconImageDefault();

		void setIconImage(const Image & imageData);

		void setIconImage(void * imageData, int width, int height, int bitsPerPixel, int stride, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);

		void setHidden(bool minimized);

		bool setMinimized(bool minimized = true);

		bool setMaximized(bool maximized = true);

		uint32 getFlags();

		int setOpacity(float opacity);

		float getOpacity();

		int toggleFakeFullscreen();

		int toggleFullscreen();

		int getScreenRectWidth() const;

		int getScreenRectHeight() const;

		int setWindowed();

		void setBordered(bool state = true);

		int setRelativeMouseMode(bool mode = true);

		void setBorderless();

		void createAndSetCursor(SDL_Cursor * cursor, SDL_SystemCursor type);

		void SetCursor(MouseCursor cursor);

		int setCursorVisible(bool show = true);

		bool isCursorVisible();

		void toggleCursor();

		void hideCursor();

		void showCursor();

		int setResizable(bool resizable = true);

		void setDimensions(int32 x, int32 y, int32 width, int32 height);

		void alert(const char * message, const char * title);

		void focus();

		uint32 getID();

		void handleResize(int32 width, int32 height);

		bool pollEvents();

		bool isKeyDown(const char * keyName);

		bool isKeyDown(SDL_Keycode keyCode);

		bool isKeyUp(const char * keyName);

		bool isKeyUp(SDL_Keycode keyCode);

		bool isKeyScanDown(const char * keyName);

		bool isKeyScanDown(SDL_Scancode keyCode);

		bool isKeyScanUp(const char * keyName);

		bool isKeyScanUp(SDL_Scancode keyCode);

		inline int getX() const 
		{
			return m_WindowInformation.location.x;
		}

		inline int getY() const 
		{
			return m_WindowInformation.location.y;
		}

		inline int getWidth() const 
		{
			return m_WindowInformation.size.width;
		}

		inline int getHeight() const 
		{
			return m_WindowInformation.size.height;
		}

		inline float getAspect() const 
		{
			return m_WindowInformation.size.aspect;
		}

		inline int getDisplayWidth() const 
		{
			return m_displayBounds.w;
		}

		inline int getDisplayHeight() const 
		{
			return m_displayBounds.h;
		}

		inline bool isBordered() const 
		{
			return m_WindowInformation.border == WindowBorder::bordered;
		}

		inline bool isBorderless() const 
		{
			return m_WindowInformation.border == WindowBorder::borderless;
		}

		inline bool isMinimized() const 
		{
			return m_WindowInformation.trayState == WindowState::minimazed;
		}

		inline bool isMaximized() const 
		{
			return m_WindowInformation.trayState == WindowState::maximized;
		}

		inline bool isHidden() const 
		{
			return m_WindowInformation.visibility == WindowVisibility::hidden;
		}

		inline bool isVisible() const 
		{
			return m_WindowInformation.visibility == WindowVisibility::visible;
		}

		inline bool Window::isFullscreen() const
		{
			return m_WindowInformation.mode == WindowMode::Fullscreen;
		}

		inline bool Window::isFakeFullscreen() const
		{
			return m_WindowInformation.mode == WindowMode::fakeFullscreen;
		}

		inline bool Window::isWindowed() const
		{
			return m_WindowInformation.mode == WindowMode::Windowed;
		}

		static void checkSDLError(int line = -1);

	private:
		MouseCursor m_currentCursor = MouseCursor::DEFAULT;

		// The window we'll be rendering to.
		SDL_Window* m_sdlWindow = nullptr;

		// Screen dimensions (Two corners of window rectangle).
		SDL_Rect m_screenRect = { 0, 0, 100, 100 };

		SDL_Rect m_displayBounds = { 0, 0, 100, 100 };

		WindowInfo m_WindowInformation;

		// An opaque handle to an OpenGL context.
		SDL_GLContext m_sdlGlContext;

		// maps the device index to indices in the devices array;
		std::map<unsigned int, unsigned int> deviceMap;
		const int JOYSTICK_DEAD_ZONE = 8000;
		SDL_Joystick* m_gameController = nullptr;
		int numJoystickButtons = 0;
		int numJoystickAxes = 0;
		int numJoystickBalls = 0;
		const char * joystickName = nullptr;
		char joystickGUIDs[33];

		// Number of physical keys on keyboard
		int numKeysOnKeyboard = 0;

		// @note Scancodes represent the physical position of the keys, 
		//		 modeled after a standard QWERTY keyboard, 
		//		 while Keycodes are the character obtained by pressing the key.
		// @note prefer keycodes where possible
		// @note array of scancodes, dont mix with keycodes!
		// @note  scancode is the key in the US QWERTY keyboard (always the same key position)! 
		// @see For available key scancodes https://wiki.libsdl.org/SDL_Scancode
		// Used in functions like "isKeyUp"
		// Allows to shorten switch and remember (fast check) released keys 
		bool* m_keyDownScancodes = nullptr;

		// X coordinate, relative to window
		int m_mouseX = 0;
		// Y coordinate, relative to window
		int m_mouseY = 0;
		int m_lastMouseX = 0;
		int m_lastMouseY = 0;

		SDL_Finger* finger;
		float m_touchX = 0;
		float m_touchY = 0;
		float m_lastTouchX = 0;
		float m_lastTouchY = 0;

		int m_lastmouseWheelX = 0;
		int m_lastmouseWheelY = 0;
		int m_mouseWheelX = 0;
		int m_mouseWheelY = 0;
	};
}