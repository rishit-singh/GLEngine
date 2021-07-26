#pragma once

#define WINDOW_H

#include "globjects.h"
#include "defaults.h"
#include "camera.h"

namespace GLEngine
{	
	typedef Point2D WindowResolution;
	// void FrameBufferSizeCallBack(GLFWwindow* window, int height, int width);

	void SetDeltaTime(float, float*, float*, float*);	//	returns the delta time for the given camera speed

	extern Camera WindowCamera;	//	Global camera instance.

	inline bool FirstMouse = true;	//	mouse glitch workaround for mouse event callback function

	inline Point2Df LastMousePosition = Point2Df(1280 / 2, 720 / 2);	//	Real time mouse position

	class Window	
	{
	public:
		static std::vector<Window*> WindowStack; 

		unsigned long ID; //	Window ID

		String Title;	//	Window title
		WindowResolution Resolution; //	Window resolution
		Color BackgroundColor; //	Window background color coordinates.
		
		GLFWwindow* GLWindow;	//	GLFWwindow pointer 		

		static void FrameBufferSizeCallBack(GLFWwindow*, int, int);	// GLFW FrameBufferSizeCallBack function
		static void MouseCallback(GLFWwindow*, double, double);	//	Moust event callback
		static void ScrollCallBack(GLFWwindow*, double, double);
		
		void Close();	//	Closes the current window instance 
		void ProcessInput(Camera&); //	Processes input events
		// Constructors forward declared for global variables depending upon this class to work.

		Window();	
		Window(bool);
		Window(String);
		Window(String, Color);
		Window(String, Point2D, Color);
		Window(Point2D, Color);

		~Window();
	};	

	extern std::vector<Window*> AllocatedWindows;	//	Stores all the window instances allocated at runtime

	// std::vector<Window*> WindowStack = std::vector<Window*>();
}