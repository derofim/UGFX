// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "Window.h"

#include "Memory.h"
#include "Log.h"
#include "Exagine.h"
#include "Image.h"
#include "exa.h"

namespace exa
{
	SDL_Cursor* SDLMouse::arrowCursor = 0;
	SDL_Cursor* SDLMouse::crosshairCursor = 0;
	SDL_Cursor* SDLMouse::moveCursor = 0;
	SDL_Cursor* SDLMouse::pointerCursor = 0;
	SDL_Cursor* SDLMouse::resizeNESWCursor = 0;
	SDL_Cursor* SDLMouse::resizeNSCursor = 0;
	SDL_Cursor* SDLMouse::resizeNWSECursor = 0;
	SDL_Cursor* SDLMouse::resizeWECursor = 0;
	SDL_Cursor* SDLMouse::textCursor = 0;
	SDL_Cursor* SDLMouse::waitCursor = 0;
	SDL_Cursor* SDLMouse::waitArrowCursor = 0;

	void Window::checkSDLError(int line)
	{
		const char* error = SDL_GetError();
		if ((error != nullptr) && (error[0] != '\0')) {
			log::error("SDL Error: %s", error);

			if (line != -1) {
				log::error("Line: %d", line);
			}

			// SDL_ClearError deletes all information about the last internal SDL error. 
			// Useful if the error has been handled by the program.
			SDL_ClearError();
		}
	}

	Window::Window()
	{
		log::debug("Creating window");
	}

	Window::~Window()
	{
		SDL_GL_DeleteContext(m_sdlGlContext);

		if (m_gameController != nullptr)
		{
			SDL_JoystickClose(m_gameController);
			m_gameController = nullptr;
		}

		SafeDelete(m_keyDownScancodes);

		SDL_DestroyWindow(m_sdlWindow);
		m_sdlWindow = nullptr;

		// Quit SDL subsystems
		// @note You shouldn't call any SDL function after SDL_Quit
		SDL_Quit();
	}

	void Window::setTitle(const char* title)
	{
		if (!m_WindowInformation.title || strlen(m_WindowInformation.title) < 0) {
			log::warning("Tried to set empty window title");
		}
		m_WindowInformation.title = title;
		SDL_SetWindowTitle(m_sdlWindow, getWindowTitle());
		checkSDLError(__LINE__);
	}

	const char* Window::getWindowTitle() const
	{
		if (!m_WindowInformation.title || strlen(m_WindowInformation.title) < 0) {
			log::warning("Returned empty window title");
			return "Exagine";
		}
		return m_WindowInformation.title;
	}

	void printAudioDevices(int iscapture)
	{
		const char *typestr = ((iscapture) ? "capture" : "output");
		int n = SDL_GetNumAudioDevices(iscapture);

		log::debug("%s devices:\n", typestr);

		if (n == -1)
			log::debug("  Driver can't detect specific %s devices.\n\n", typestr);
		else if (n == 0)
			log::debug("  No %s devices found.\n\n", typestr);
		else {
			int i;
			for (i = 0; i < n; i++) {
				log::debug("  %s\n", SDL_GetAudioDeviceName(i, iscapture));
			}
			log::debug("\n\n");
		}
	}

	bool Window::init() 
	{
		log::debug("Initializing window\n\n");
		
		// Initialize SDL
		// @note SDL_Init must be called before using any other SDL function
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
		{
			log::error("SDL could not initialize!");
			checkSDLError(__LINE__);
			return false;
		}

		// @see http://wiki.libsdl.org/moin.cgi/SDL_GetVersion
		//		http://wiki.libsdl.org/moin.cgi/SDL_GetRevision
		//		http://wiki.libsdl.org/moin.cgi/SDL_GetRevisionNumber
		SDL_version linked;
		SDL_GetVersion(&linked);
		log::debug("SDL v%d.%d.%d", linked.major, linked.minor, linked.patch);
		log::debug("SDL revision hash: %s", SDL_GetRevision());
		log::debug("SDL revision number: %d", SDL_GetRevisionNumber());
		log::debug("\n\n");

		/**
		* @see
		*    http://wiki.libsdl.org/moin.cgi/SDL_GetPlatform
		*    http://wiki.libsdl.org/moin.cgi/SDL_GetCPUCount
		*    http://wiki.libsdl.org/moin.cgi/SDL_GetCPUCacheLineSize
		**/
		log::debug("=== Platform info ===");
		log::debug("Platform: %s", SDL_GetPlatform());
		log::debug("CPU count: %d", SDL_GetCPUCount());
		log::debug("CPU cache line size: %d", SDL_GetCPUCacheLineSize());
		log::debug("\n\n");

		/**
		* @brief Tests SDL_HasXYZ() functions
		* @see 
		* http://wiki.libsdl.org/moin.cgi/SDL_Has3DNow
		* http://wiki.libsdl.org/moin.cgi/SDL_HasAltiVec
		* http://wiki.libsdl.org/moin.cgi/SDL_HasMMX
		* http://wiki.libsdl.org/moin.cgi/SDL_HasRDTSC
		* http://wiki.libsdl.org/moin.cgi/SDL_HasSSE
		* http://wiki.libsdl.org/moin.cgi/SDL_HasSSE2
		* http://wiki.libsdl.org/moin.cgi/SDL_HasSSE3
		* http://wiki.libsdl.org/moin.cgi/SDL_HasSSE41
		* http://wiki.libsdl.org/moin.cgi/SDL_HasSSE42
		* http://wiki.libsdl.org/moin.cgi/SDL_HasAVX
		**/
		log::debug("Has RDTSC: %s", SDL_HasRDTSC() ? "true" : "false");
		log::debug("Has AltiVec: %s", SDL_HasAltiVec() ? "true" : "false");
		log::debug("Has MMX: %s", SDL_HasMMX() ? "true" : "false");
		log::debug("Has 3DNow: %s", SDL_Has3DNow() ? "true" : "false");
		log::debug("Has SSE: %s", SDL_HasSSE() ? "true" : "false");
		log::debug("Has SSE2: %s", SDL_HasSSE2() ? "true" : "false");
		log::debug("Has SSE3: %s", SDL_HasSSE3() ? "true" : "false");
		log::debug("Has SSE41: %s", SDL_HasSSE41() ? "true" : "false");
		log::debug("Has SSE42: %s", SDL_HasSSE42() ? "true" : "false");
		log::debug("Has AVX: %s", SDL_HasAVX() ? "true" : "false");
		log::debug("\n\n");

		// Get the current power supply details.
		// @see  http://wiki.libsdl.org/moin.cgi/SDL_GetPowerInfo
		SDL_PowerState state;
		int secs; // seconds of battery life left, will return -1 if we can't determine a value
		int pct; // percentage of battery life left, will return -1 if we can't determine a value
		state = SDL_GetPowerInfo(&secs, &pct);
		log::debug("Has power battery: %s", state == SDL_POWERSTATE_ON_BATTERY ? "true" : "false");
		log::debug("\n\n");

		/* Print available audio drivers */
		int n = SDL_GetNumAudioDrivers();
		if (n == 0) {
			log::debug("No built-in audio drivers\n\n");
		}
		else {
			int i;
			log::debug("Built-in audio drivers:\n");
			for (i = 0; i < n; ++i) {
				log::debug("  %s\n", SDL_GetAudioDriver(i));
			}
			log::debug("\n\n");
		}
		log::debug("Using current audio driver: %s\n\n", SDL_GetCurrentAudioDriver());

		printAudioDevices(0);
		printAudioDevices(1);

		// A hint that specifies whether the Android / iOS built-in accelerometer 
		// should be listed as a joystick device, rather than listing actual joysticks only. 
		SDL_SetHint(SDL_HINT_ACCELEROMETER_AS_JOYSTICK, "1");

		// Check for joysticks
		if (SDL_NumJoysticks() < 1)
		{
			log::warning("Warning: No joysticks connected!");
		}
		else
		{
			auto numJoysticks = SDL_NumJoysticks();
			log::debug("%d joysticks found", numJoysticks);

			// Get index of working joystick
			int connectToJoystickIndex = 0;
			// @see https://www.libsdl.org/tmp/SDL/test/testjoystick.c
			for (int i = 0; i < numJoysticks; ++i) {
				const char* name = SDL_JoystickNameForIndex(i);
				log::debug("Joystick %d: %s", i, name ? name : "Unknown Joystick");
				
				// Load joystick
				m_gameController = SDL_JoystickOpen(i);
				if (m_gameController == nullptr)
				{
					log::warning("Warning: Unable to open game controller!");
					checkSDLError(__LINE__);
				} else {
					connectToJoystickIndex = i;

					// @see https://gitlab.cg.tuwien.ac.at/Space-Walk/space-walk/blob/e703017400df643adf84af66b8a72fd5ab073456/tools/input-logger-cpp/src/JoystickManager.hpp
					auto deviceindex = SDL_JoystickInstanceID(m_gameController);
					deviceMap.insert(std::pair<unsigned int, unsigned int>(deviceindex, i));
				}
				SDL_JoystickClose(m_gameController);
			}

			m_gameController = SDL_JoystickOpen(connectToJoystickIndex);
			if (m_gameController == nullptr)
			{
				log::warning("Warning: Unable to open game controller!");
				checkSDLError(__LINE__);
			}



			// @see http://docs.ros.org/indigo/api/joystick_sdl/html/joystick_8cpp_source.html
			numJoystickAxes = SDL_JoystickNumAxes(m_gameController);
			numJoystickButtons = SDL_JoystickNumButtons(m_gameController);
			numJoystickBalls = SDL_JoystickNumBalls(m_gameController);
			joystickName = SDL_JoystickName(m_gameController);
			
			SDL_JoystickGetGUIDString(SDL_JoystickGetDeviceGUID(connectToJoystickIndex), joystickGUIDs, sizeof(joystickGUIDs));

			log::debug("Connected to joystick %s", joystickName);
			log::debug("Opened Joystick %d", connectToJoystickIndex);
			log::debug("Name: %s", joystickName);
			log::debug("Number of Axes: %d", numJoystickAxes);
			log::debug("Number of Buttons: %d", numJoystickButtons);
			log::debug("Number of Balls: %d", numJoystickBalls);
			log::debug("Joystick GUID %s", joystickGUIDs);

		}

		// Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			log::warning("Warning: Linear texture filtering not enabled!");
			checkSDLError(__LINE__);
		}

		if (!SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, "1", SDL_HINT_OVERRIDE))
		{
			log::warning("Warning: SDL_HINT_RENDER_VSYNC not enabled!");
			checkSDLError(__LINE__);
		}

		// Refreshes info about device display mode
		recalculateDisplayModeRect();

		Uint32 flags = SDL_WINDOW_OPENGL;

		if (isFullscreen()) {
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		// Create window
		m_sdlWindow = SDL_CreateWindow(getWindowTitle(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			getScreenRectWidth(), getScreenRectHeight(),
			flags);

		if (m_sdlWindow == nullptr)
		{
			log::error("Window could not be created!");
			checkSDLError(__LINE__);
			return false;
		}

		recalculateDisplayBounds();

		// Refreshes info about window size 
		recalculateWindowSize();

		// While the mouse is in relative mode, the cursor is hidden, 
		// and the driver will try to report continuous motion in the current window.
		// Only relative motion events will be delivered, the mouse position will not change.
		setRelativeMouseMode(true);

		// Hide cursor
		// @note SDL_SetRelativeMouseMode also hides cursor
		hideCursor();

		// When input is grabbed the mouse is confined to the window.
		// If the caller enables a grab while another window is currently grabbed, 
		// the other window loses its grab in favor of the caller's window.
		SDL_SetWindowGrab(m_sdlWindow, SDL_TRUE);

		// Load default OpenGL.
		SDL_GL_LoadLibrary(nullptr);

		// Draw directly on the screen using double buffer. Double buffer makes the renderer smoother.
		// @note: The front buffer contains the final output image
		// that is shown at the screen, while all the rendering commands draw to the back buffer.
		// As soon as all the rendering commands are finished we swap the back buffer to the front
		// buffer so the image is instantly displayed to the user, removing all rendering artifacts.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		// If you don't call this, there will be no stencil buffer. 
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		// Set size of buffers
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

		// TODO: https://github.com/openfl/lime/blob/efa7c0eab6596619e7414d046a35fdb817ffab96/project/src/backend/sdl/SDLWindow.cpp#L68
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

		// Request an OpenGL context
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		m_sdlGlContext = SDL_GL_CreateContext(m_sdlWindow);

		if (!m_sdlGlContext) {
			log::error("Error: Failed to create context");
			checkSDLError(__LINE__);
			return false;
		}

		//Use Vsync
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			log::warning("Warning: Unable to set VSync!");
			checkSDLError(__LINE__);
		}

		// Load OpenGL functions
		gladLoadGLLoader(SDL_GL_GetProcAddress);

		// Print OpenGL version using glad.		   
		log::debug("OpenGL %d.%d", GLVersion.major, GLVersion.minor);

		// Get information about context 
		log::debug("OpenGL Vendor:   %s", exaglGetString(GL_VENDOR));
		log::debug("OpenGL Renderer: %s", exaglGetString(GL_RENDERER));
		log::debug("GLSL Version: %s", exaglGetString(GL_SHADING_LANGUAGE_VERSION));
		log::debug("OpenGL Version:  %s", exaglGetString(GL_VERSION));

		if (!GLAD_GL_EXT_vertex_array) {
			log::warning("OpenGL Warning: GLAD_GL_EXT_vertex_array not supported");
		}

		log::debug("\n\n");

		// Re-enabling Vertical Sync if necessary
		// @see Flags SDL_GL_DOUBLEBUFFER and SDL_RENDERER_PRESENTVSYNC in SDL_CreateWindow
		if (SDL_GL_GetSwapInterval() == 0) {
			log::debug("Re-enabling Vertical Sync");
			// Try to enable Vsync (late swap tearing mode) 
			if (SDL_GL_SetSwapInterval(-1) < 0) {
				//Try enable Vsync (updates synchronized with the vertical retrace mode) 
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					log::warning("Warning: Unable to set VSync!");
					checkSDLError(__LINE__);
				}
			}
		}

		int Buffers, Samples;
		SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &Buffers);
		SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &Samples);
		log::debug("SDL_GL_MULTISAMPLEBUFFERS = %d", Buffers);
		log::debug("SDL_GL_MULTISAMPLESAMPLES = %d", Samples);

		int r, g, b;
		SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &r);
		SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &g);
		SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &b);

		log::debug("SDL_GL_RED_SIZE = %d", r);
		log::debug("SDL_GL_GREEN_SIZE = %d", g);
		log::debug("SDL_GL_BLUE_SIZE = %d", b);

		const Uint8* m_SDLKeyState = SDL_GetKeyboardState(&numKeysOnKeyboard);
		m_keyDownScancodes = new bool[numKeysOnKeyboard]();

		log::debug("Number of keys on keyboard by SDL_GetKeyboardState is %d", numKeysOnKeyboard);
		log::debug("\n\n");

		return true;
	}

	void Window::swapWindow()
	{
		SDL_GL_SwapWindow(m_sdlWindow);
	}

	void Window::setSize(int32 width, int32 height)
	{
		handleResize(width, height);
		SDL_SetWindowSize(m_sdlWindow, width, height);
	}

	void Window::addSize(int32 x, int32 y)
	{
		setSize(getWidth() + x, getHeight() + y);
	}

	void Window::setWidth(int32 x)
	{
		setSize(x, getHeight());
	}

	// get the index of the display associated with a window.
	int Window::getWindowDisplayIndex()
	{
		return SDL_GetWindowDisplayIndex(m_sdlWindow);
	}

	// move the mouse to the given position within the window.
	void Window::moveMouse(int x, int y) {
		SDL_WarpMouseInWindow(m_sdlWindow, x, y);
	}

	// move the mouse to the given position in global screen space.
	void Window::moveMouseGlobal(int x, int y) {
		SDL_WarpMouseGlobal(x, y);
	}

	void Window::acceptTextInput(bool enabled)
	{
		if (enabled) {
			// This function will start accepting Unicode text input events in the 
			// focused SDL window, and start emitting SDL_TEXTINPUT and SDL_TEXTEDITING events.
			SDL_StartTextInput();
		} else {
			// Use this function to stop receiving any text input events.
			SDL_StopTextInput();
		}
	}

	bool Window::isAcceptingTextInput()
	{
		// check whether or not Unicode text input events are enabled.
		return static_cast<bool>(SDL_IsTextInputActive());
	}

	void Window::setHeight(int32 y)
	{
		setSize(getWidth(), y);
	}

	void Window::handleReposition(int32 x, int32 y)
	{
		m_WindowInformation.location.x = x;
		m_WindowInformation.location.y = y;
		if (x <= 0 || y <= 0) {
			log::warning("Tried to set position <= 0");
		}
	}

	void Window::setPosition(int32 x, int32 y)
	{
		handleReposition(x,y);
		SDL_SetWindowPosition(m_sdlWindow, x, y);
	}

	void Window::moveBy(int32 x, int32 y)
	{
		setPosition(getX() + x, getY() + y);
	}

	void Window::setX(int32 x)
	{
		setPosition(x, getY());
	}

	void Window::setY(int32 y)
	{
		setPosition(getX(), y);
	}

	bool Window::setFullscreen()
	{
		if (!isFullscreen()) {
			return toggleFullscreen() >= 0;
		} else {
			log::warning("Tried to go fullscreen while in fullscreen mode");
		}
		return true;
	}

	bool Window::recalculateDisplayBounds() 
	{
		int idx = getWindowDisplayIndex();
		if (SDL_GetDisplayBounds(idx, &m_displayBounds) != 0) {
			log::error("SDL_GetDisplayBounds failed");
			checkSDLError(__LINE__);
			return false;
		}
		return true;
	}

	/**
	* set a window's fullscreen state.
	* @param flags may be SDL_WINDOW_FULLSCREEN, for "real" fullscreen with a videomode change; 
	* SDL_WINDOW_FULLSCREEN_DESKTOP for "fake" fullscreen that takes the size of the desktop; 
	* and 0 for windowed mode.
	**/
	int Window::setFullscreenMode(Uint32 flags) 
	{
		int result = SDL_SetWindowFullscreen(m_sdlWindow, flags);
		if (result < 0) {
			log::error("Failure trying to set fullscreen mode!");
			checkSDLError(__LINE__);
		}
		return result;
	}

	bool Window::isResizable() 
	{
		return static_cast<bool>(getFlags() & SDL_WINDOW_RESIZABLE);
	}

	// restore the size and position of a minimized or maximized window.
	void Window::RestoreDimentions()
	{
		SDL_RestoreWindow(m_sdlWindow);
	}

	void Window::setIcon(SDL_Surface* surface)
	{
		SDL_SetWindowIcon(m_sdlWindow, surface);
	}

	void Window::setIconImageDefault() 
	{
		// raw pixel data
		Uint16 pixels[16 * 16] = {
			0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
			0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
			0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
			0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
			0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
			0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
			0x0fff, 0x0aab, 0x0789, 0x0bcc, 0x0eee, 0x09aa, 0x099a, 0x0ddd,
			0x0fff, 0x0eee, 0x0899, 0x0fff, 0x0fff, 0x1fff, 0x0dde, 0x0dee,
			0x0fff, 0xabbc, 0xf779, 0x8cdd, 0x3fff, 0x9bbc, 0xaaab, 0x6fff,
			0x0fff, 0x3fff, 0xbaab, 0x0fff, 0x0fff, 0x6689, 0x6fff, 0x0dee,
			0xe678, 0xf134, 0x8abb, 0xf235, 0xf678, 0xf013, 0xf568, 0xf001,
			0xd889, 0x7abc, 0xf001, 0x0fff, 0x0fff, 0x0bcc, 0x9124, 0x5fff,
			0xf124, 0xf356, 0x3eee, 0x0fff, 0x7bbc, 0xf124, 0x0789, 0x2fff,
			0xf002, 0xd789, 0xf024, 0x0fff, 0x0fff, 0x0002, 0x0134, 0xd79a,
			0x1fff, 0xf023, 0xf000, 0xf124, 0xc99a, 0xf024, 0x0567, 0x0fff,
			0xf002, 0xe678, 0xf013, 0x0fff, 0x0ddd, 0x0fff, 0x0fff, 0xb689,
			0x8abb, 0x0fff, 0x0fff, 0xf001, 0xf235, 0xf013, 0x0fff, 0xd789,
			0xf002, 0x9899, 0xf001, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0xe789,
			0xf023, 0xf000, 0xf001, 0xe456, 0x8bcc, 0xf013, 0xf002, 0xf012,
			0x1767, 0x5aaa, 0xf013, 0xf001, 0xf000, 0x0fff, 0x7fff, 0xf124,
			0x0fff, 0x089a, 0x0578, 0x0fff, 0x089a, 0x0013, 0x0245, 0x0eff,
			0x0223, 0x0dde, 0x0135, 0x0789, 0x0ddd, 0xbbbc, 0xf346, 0x0467,
			0x0fff, 0x4eee, 0x3ddd, 0x0edd, 0x0dee, 0x0fff, 0x0fff, 0x0dee,
			0x0def, 0x08ab, 0x0fff, 0x7fff, 0xfabc, 0xf356, 0x0457, 0x0467,
			0x0fff, 0x0bcd, 0x4bde, 0x9bcc, 0x8dee, 0x8eff, 0x8fff, 0x9fff,
			0xadee, 0xeccd, 0xf689, 0xc357, 0x2356, 0x0356, 0x0467, 0x0467,
			0x0fff, 0x0ccd, 0x0bdd, 0x0cdd, 0x0aaa, 0x2234, 0x4135, 0x4346,
			0x5356, 0x2246, 0x0346, 0x0356, 0x0467, 0x0356, 0x0467, 0x0467,
			0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
			0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
			0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
			0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff
		};
		setIconImage(pixels, 16, 16, 16, 16 * 2, 0x0f00, 0x00f0, 0x000f, 0xf000);
	}

	void Window::setIconImage(const Image &image)
	{

		int req_format = image.getRequestedFormat();

		// Set up the pixel format color masks for RGB(A) byte arrays.
		// Only STBI_rgb (3) and STBI_rgb_alpha (4) are supported here!
		Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		int shift = (req_format == STBI_rgb) ? 8 : 0;
		rmask = 0xff000000 >> shift;
		gmask = 0x00ff0000 >> shift;
		bmask = 0x0000ff00 >> shift;
		amask = 0x000000ff >> shift;
#else // little endian, like x86
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = (req_format == EXA_rgb) ? 0 : 0xff000000;
#endif

		int depth, pitch;
		if (req_format == EXA_rgb) {
			depth = 24;
			pitch = 3 * image.getWidth(); // 3 bytes per pixel * pixels per row
		}
		else { // STBI_rgb_alpha (RGBA)
			depth = 32;
			pitch = 4 * image.getWidth();
		}

		setIconImage(image.getData(), image.getWidth(), image.getHeight(), depth, pitch,
			rmask, gmask, bmask, amask);
	}

	void Window::setIconImage(void* imageData, int width, int height, int bitsPerPixel, int stride,
		Uint32 Rmask = 0x000000FF, Uint32 Gmask = 0x0000FF00, Uint32 Bmask = 0x00FF0000, 
		Uint32 Amask = 0xFF000000)
	{
		SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
			/* a pointer to existing pixel data */ imageData, 
			/* the width of the surface */ width,
			/* the height of the surface */ height,
			/* the depth of the surface in bits  */ bitsPerPixel, 
			/* the length of a row of pixels in bytes, pitch */ stride, 
			/* the red mask for the pixels */ Rmask,
			/* the green mask for the pixels */ Gmask,
			/* the blue mask for the pixels */ Bmask,
			/* the alpha mask for the pixels */ Amask
		);

		if (surface) {
			setIcon(surface);
			SDL_FreeSurface(surface);
		}
	}

	void Window::setHidden(bool hide)
	{
		if (hide) {
			SDL_HideWindow(m_sdlWindow);
			m_WindowInformation.visibility = WindowVisibility::hidden;
		}
		else {
			SDL_ShowWindow(m_sdlWindow);
			m_WindowInformation.visibility = WindowVisibility::visible;
		}
	}

	bool Window::setMinimized(bool minimized)
	{
		if (minimized) {
			SDL_MinimizeWindow(m_sdlWindow);
			m_WindowInformation.trayState = WindowState::minimazed;
		} else {
			RestoreDimentions();
			m_WindowInformation.trayState = WindowState::maximized;
		}
		return minimized;
	}

	bool Window::setMaximized(bool maximized)
	{
		if (maximized) {
			SDL_MaximizeWindow(m_sdlWindow);
		}
		else {
			RestoreDimentions();
		}
		return maximized;
	}

	uint32 Window::getFlags() 
	{
		return SDL_GetWindowFlags(m_sdlWindow);
	}

	// Sets the opacity value (0.0f - transparent, 1.0f - opaque)
	int Window::setOpacity(float opacity)
	{
		int result = SDL_SetWindowOpacity(m_sdlWindow, opacity);
		if (result < 0) {
			log::error("Failure trying to set window opacity!");
			checkSDLError(__LINE__);
		}
		return result;
	}

	// Gets the opacity value (0.0f - transparent, 1.0f - opaque)
	float Window::getOpacity()
	{
		float opacity = 1.0f;
		int result = SDL_GetWindowOpacity(m_sdlWindow, &opacity);
		if (result < 0) {
			log::error("Failure trying to set window opacity!");
			checkSDLError(__LINE__);
		}
		return opacity;
	}

	// "fake" fullscreen that takes the size of the desktop
	int Window::toggleFakeFullscreen()
	{
		auto flags = getFlags();
		flags ^= SDL_WINDOW_FULLSCREEN_DESKTOP;
		int result = setFullscreenMode(flags);

		// Set window in fullscreen mode
		if ((flags & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
		{
			m_WindowInformation.mode = WindowMode::Fullscreen;
			// Set window size to screen size
			// @note also need to reset position 
			setDimensions(0, 0, getDisplayWidth(), getDisplayHeight());
		}
		else {
			m_WindowInformation.mode = WindowMode::Windowed;
		}
		return result;
	}

	int Window::toggleFullscreen() 
	{
		auto flags = getFlags();
		flags ^= SDL_WINDOW_FULLSCREEN;
		int result = setFullscreenMode(flags);

		// Set window in fullscreen mode
		if ((flags & SDL_WINDOW_FULLSCREEN) != 0)
		{
			m_WindowInformation.mode = WindowMode::Fullscreen;
			// Set window size to screen size
			// @note also need to reset position 
			setDimensions(0, 0, getDisplayWidth(), getDisplayHeight());
		}
		else {
			m_WindowInformation.mode = WindowMode::Windowed;
		}
		return result;
	}

	// Monitor width
	int Window::getScreenRectWidth() const {
		return  m_screenRect.w - m_screenRect.x;
	}

	// Monitor height
	int Window::getScreenRectHeight() const 
	{
		return  m_screenRect.h - m_screenRect.y;
	}

	int  Window::setWindowed()
	{
		int result = setFullscreenMode(0);
		return result;
	}

	// This will add or remove the window's SDL_WINDOW_BORDERLESS flag 
	// and add or remove the border from the actual window. 
	// This is a no-op if the window's border already matches the requested state.
	// @note You can't change the border state of a fullscreen window.
	void Window::setBordered(bool bordered) 
	{
		SDL_SetWindowBordered(m_sdlWindow, bordered ? SDL_TRUE : SDL_FALSE );
		m_WindowInformation.border = bordered ? WindowBorder::bordered : WindowBorder::borderless;
		
		if (!bordered) {
			// @note Sets app to windowed mode.
			showCursor();
			// 0 for windowed mode.
			setWindowed();
		}
	}

	// While the mouse is in relative mode, the cursor is hidden, 
	// and the driver will try to report continuous motion in the current window.
	// Only relative motion events will be delivered, the mouse position will not change.
	int Window::setRelativeMouseMode(bool mode)
	{
		int result = SDL_SetRelativeMouseMode(mode ? SDL_TRUE : SDL_FALSE);
		if (result < 0) {
			log::error("Failure trying to toggle whether or not the cursor is shown!");
			checkSDLError(__LINE__);
		}
		return result;
	}

	void Window::setBorderless()
	{
		setBordered(false);
	}

	void Window::createAndSetCursor(SDL_Cursor* cursor, SDL_SystemCursor type) 
	{
		if (cursor == nullptr)
		{
			cursor = SDL_CreateSystemCursor(type);
		}

		SDL_SetCursor(cursor);
	}

	void Window::SetCursor(MouseCursor cursor) {
		if (cursor != m_currentCursor) 
		{
			switch (cursor) 
			{
			case MouseCursor::CROSSHAIR:
				createAndSetCursor(SDLMouse::crosshairCursor, SDL_SYSTEM_CURSOR_CROSSHAIR);
				break;

			case MouseCursor::MOVE:
				createAndSetCursor(SDLMouse::moveCursor, SDL_SYSTEM_CURSOR_SIZEALL);
				break;

			case MouseCursor::POINTER:
				createAndSetCursor(SDLMouse::pointerCursor, SDL_SYSTEM_CURSOR_HAND);
				break;

			case MouseCursor::RESIZE_NESW:
				createAndSetCursor(SDLMouse::resizeNESWCursor, SDL_SYSTEM_CURSOR_SIZENESW);
				break;

			case MouseCursor::RESIZE_NS:
				createAndSetCursor(SDLMouse::resizeNSCursor, SDL_SYSTEM_CURSOR_SIZENS);
				break;

			case MouseCursor::RESIZE_NWSE:
				createAndSetCursor(SDLMouse::resizeNWSECursor, SDL_SYSTEM_CURSOR_SIZENWSE);
				break;

			case MouseCursor::RESIZE_WE:
				createAndSetCursor(SDLMouse::resizeWECursor, SDL_SYSTEM_CURSOR_SIZEWE);
				break;

			case MouseCursor::TEXT:
				createAndSetCursor(SDLMouse::textCursor, SDL_SYSTEM_CURSOR_IBEAM);
				break;

			case MouseCursor::WAIT:
				createAndSetCursor(SDLMouse::waitCursor, SDL_SYSTEM_CURSOR_WAIT);
				break;

			case MouseCursor::WAIT_ARROW:
				createAndSetCursor(SDLMouse::waitArrowCursor, SDL_SYSTEM_CURSOR_WAITARROW);
				break;

			default:
				createAndSetCursor(SDLMouse::arrowCursor, SDL_SYSTEM_CURSOR_ARROW);
				break;

			}
			m_currentCursor = cursor;
		}
	}

	int Window::setCursorVisible(bool show) 
	{
		int result = SDL_ShowCursor(show ? SDL_ENABLE : SDL_DISABLE);
		if (result < 0) {
			log::error("Failure trying to toggle whether or not the cursor is shown!");
			checkSDLError(__LINE__);
		}
		else {
			// While the mouse is in relative mode, the cursor is hidden, so
			// need to reset mouse mode	if cursor must be shown
			if (show) {
				setRelativeMouseMode(false);
			}
		}
		return result;
	}

	bool Window::isCursorVisible() 
	{
		// SDL_QUERY to query the current state
		int result = SDL_ShowCursor(SDL_QUERY);
		if (result < 0) {
			log::error("Failure trying to know whether or not the cursor is shown!");
			checkSDLError(__LINE__);
		}
		else if (result == SDL_ENABLE) {
			// cursor is shown
			return true;
		}

		// assume that cursor is hidden
		return false;
	}

	void Window::toggleCursor() 
	{
		isCursorVisible() ? hideCursor() : showCursor();
	}

	void Window::hideCursor()
	{
		if (isCursorVisible()) {
			setCursorVisible(false);
		}
	}

	void Window::showCursor()
	{
		if (!isCursorVisible()) {
			setCursorVisible(true);
		}
	}

	int Window::setResizable(bool resizable)
	{
		SDL_SetWindowResizable(m_sdlWindow, resizable ? SDL_TRUE : SDL_FALSE);
		int result = setWindowed();
		return result;
	}

	void Window::setDimensions(int32 x, int32 y, int32 width, int32 height)
	{
		setPosition(x, y);
		setSize(width, height);
	}

	void Window::alert(const char* message, const char* title) 
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title, message, m_sdlWindow);
	}

	void Window::focus() 
	{
		SDL_RaiseWindow(m_sdlWindow);
	}

	uint32 Window::getID()
	{
		return SDL_GetWindowID(m_sdlWindow);
	}

	void Window::handleResize(int32 width, int32 height) 
	{
		log::debug("Resizing window (%d, %d)", width, height);
		m_WindowInformation.size.width = width;
		m_WindowInformation.size.height = height;
		if (width <= 0 || height <= 0) {
			log::warning("Tried to set window size <= 0");
		}
		m_WindowInformation.size.aspect = getWidth() / static_cast<float>(getHeight());
	}

	// @see https://wiki.libsdl.org/SDL_WindowEvent
	bool Window::pollEvents()
	{
		// Event handler
		SDL_Event event;

		// Handle events on queue
		while (SDL_PollEvent(&event) != 0)
		{
			
			// Returns SDL_TRUE if SDL_QUIT is queued or SDL_FALSE otherwise.
			if (SDL_QuitRequested()) {
				EXAGINE().stop();
				return false;
			}
			
			// User requests quit
			if (event.type == SDL_QUIT) {
				EXAGINE().stop();
				return false;
			}

			if (event.type == SDL_MOUSEMOTION) {
				m_lastMouseX = m_mouseX;
				m_lastMouseY = m_mouseY;

				// @note Similar to SDL_GetRelativeMouseState(&x, &y);
				m_mouseX = event.motion.x;
				m_mouseY = event.motion.y;

				if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
					// Mouse is moving and left button pressed
				}
			}

			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
				handleResize(event.window.data1, event.window.data2);
			}

			if (event.type == SDL_KEYUP) {
				m_keyDownScancodes[event.key.keysym.scancode] = false;
				const char* scancodeName = SDL_GetScancodeName(event.key.keysym.scancode);
				const char* keycodeName = SDL_GetKeyName(event.key.keysym.sym);
				log::debug("KEYUP: Physical %s key as virtual %s key", scancodeName, keycodeName);
			}

			if (event.type == SDL_KEYDOWN) {
				m_keyDownScancodes[event.key.keysym.scancode] = true;
				const char* scancodeName = SDL_GetScancodeName(event.key.keysym.scancode);
				const char* keycodeName = SDL_GetKeyName(event.key.keysym.sym);
				log::debug("KEYDOWN: Physical %s key as virtual %s key", scancodeName, keycodeName);
			}

			if (event.type == SDL_FINGERDOWN) {
				m_lastTouchX = m_touchX;
				m_lastTouchY = m_touchY;
				m_touchX = event.tfinger.x * getWidth();
				m_touchY = event.tfinger.y * getHeight();
				int n = SDL_GetNumTouchFingers(event.tfinger.touchId);
				if (n >= 2) {
					// Multi Touch
				}
				if (n >= 1 && n < 2) {
					finger = SDL_GetTouchFinger(event.tfinger.touchId, n - 1);
					log::debug("SDL_GetTouchFinger : %f %f", finger->x, finger->y);
				}
			}

			if (event.type == SDL_MOUSEWHEEL) {
				m_lastmouseWheelX = m_mouseWheelX;
				m_lastmouseWheelY = m_mouseWheelY;
				m_mouseWheelX = event.wheel.x;
				m_mouseWheelY = event.wheel.y;
			}

			// @see https://www.libsdl.org/tmp/SDL/test/testjoystick.c
			if (event.type == SDL_JOYBALLMOTION)
			{
				log::debug("Joystick %d ball %d delta: (%d,%d)\n",
					event.jball.which,
					event.jball.ball, event.jball.xrel, event.jball.yrel);
			}

			if (event.type == SDL_JOYHATMOTION)
			{
				log::debug("Joystick %d hat %d value:",
					event.jhat.which, event.jhat.hat);
			}

			if (event.type == SDL_JOYBUTTONUP)
			{
				log::debug("Joystick %d button %d down\n",
					event.jbutton.which, event.jbutton.button);
			}

			if (event.type == SDL_JOYBUTTONDOWN)
			{
				log::debug("Joystick %d button %d up\n",
					event.jbutton.which, event.jbutton.button);
			}

			if (event.type == SDL_JOYAXISMOTION)
			{
				auto number = deviceMap[event.jdevice.which];
				auto axis = (unsigned int)event.jaxis.axis;
				auto value = (float)event.jaxis.value / (float)(0xffff / 2);

				// X axis motion
				if (event.jaxis.axis == 0)
				{
					if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
					{
						// Left of dead zone
					}
					else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
					{
						// Right of dead zone
					}
					else
					{
						// In dead zone on X axis
					}
				}
				// Y axis motion
				else if (event.jaxis.axis == 1)
				{
					if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
					{
						// Below of dead zone
					}
					else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
					{
						// Above of dead zone
					}
					else
					{
						// In dead zone on Y axis
					}
				}
			}
		}

		if (isKeyDown(SDLK_ESCAPE) || isKeyDown(SDLK_AC_BACK)) {
			EXAGINE().stop();
			return false;
		}

		if (isKeyDown("Delete")) {
			EXAGINE().stop();
			return false;
		}

		return true;
	}

	bool Window::isKeyDown(const char* keyName) 
	{
		auto key = SDL_GetKeyFromName(keyName);
		if (key == SDLK_UNKNOWN) {
			log::error("Unknown key %s", keyName);
		}
		return isKeyDown(key);
	}

	bool Window::isKeyDown(SDL_Keycode keyCode) 
	{
		// Convert virtual key to physical key position on keyboard (as in QWERTY layout)
		SDL_Scancode currentKey = SDL_GetScancodeFromKey(keyCode);
		if (isKeyScanDown(currentKey)) {
			return true;
		}
		return false;
	}

	bool Window::isKeyUp(const char* keyName) 
	{
		return !isKeyDown(keyName);
	}

	bool Window::isKeyUp(SDL_Keycode keyCode) 
	{
		return !isKeyDown(keyCode);
	}

	bool Window::isKeyScanDown(const char* keyName) 
	{
		auto key = SDL_GetScancodeFromName(keyName);
		if (key == SDL_SCANCODE_UNKNOWN) {
			log::error("Unknown key scancode %s", keyName);
		}
		return isKeyScanDown(key);
	}

	bool Window::isKeyScanDown(SDL_Scancode keyCode) 
	{
		if ((keyCode <= numKeysOnKeyboard && m_keyDownScancodes[keyCode])) {
			return true;
		}
		return false;
	}

	bool Window::isKeyScanUp(const char* keyName) 
	{
		return !isKeyScanDown(keyName);
	}

	bool Window::isKeyScanUp(SDL_Scancode keyCode) 
	{
		return !isKeyScanDown(keyCode);
	}

	// Refreshes info about device display mode
	void Window::recalculateDisplayModeRect()
	{
		//Get device display mode
		SDL_DisplayMode displayMode;
		if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0)
		{
			log::error("Failed to get SDL display mode and its size");
			checkSDLError(__LINE__);
		}  
	
		m_screenRect.w = displayMode.w;
		m_screenRect.h = displayMode.h;
	}

	// Refreshes info about window size 
	void Window::recalculateWindowSize() 
	{
		int width, height;
		SDL_GetWindowSize(m_sdlWindow, &width, &height);
		handleResize(width, height);
	}
}