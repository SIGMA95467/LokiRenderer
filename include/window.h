#pragma once
#include <Windows.h>

#include <glm/glm.hpp>

typedef struct mouse
{
				// for camera orbit
				glm::vec2 orbit_pos;
				glm::vec2 orbit_delta;
				// for first-person view (diabled now)
				glm::vec2 fv_pos;
				glm::vec2 fv_delta;
				// for mouse wheel
				float wheel_delta;
}mouse_t;

typedef struct window
{
				HWND h_window;
				HDC mem_dc;
				HBITMAP bm_old;
				HBITMAP bm_dib;
				unsigned char* window_fb;
				int width;
				int height;
				char keys[512];
				char buttons[2];	//left button¡ª0£¬ right button¡ª1
				int is_close;
				mouse_t mouse_info;
}window_t;

extern window_t* window;

int window_init(int width, int height, const char* title);
int window_destroy();
void window_draw(unsigned char* framebuffer);
void msg_dispatch();
glm::vec2 get_mouse_pos();
float platform_get_time(void);

//#include <glm/glm.hpp>
//#include <windows.h>
//
//				class WindowApp
//				{
//				public:
//								WindowApp(int w, int h, const char* title);
//								~WindowApp();
//								void window_draw(unsigned char* framebuffer);
//								glm::vec2 get_mouse_pos();
//								void msg_dispatch();
//
//								HWND h_window;
//								HDC mem_dc;
//								HBITMAP bm_old;
//								HBITMAP bm_dib;
//								unsigned char* m_window_fb;
//								int m_width;
//								int m_height;
//								char m_keys[512];
//								char m_buttons[2];	//left button¡ª0£¬ right button¡ª1
//								int m_is_close;
//				private:
//
//								void init_bm_header(BITMAPINFOHEADER& bi, int width, int height);
//								void window_display();
//
//								void register_window_class();
//				};
//
//
//				class Mouse {
//				public:
//								Mouse();
//								~Mouse();
//								// for camera orbit
//								glm::vec2 orbit_pos;
//								glm::vec2 orbit_delta;
//								// for first-person view (diabled now)
//								glm::vec2 fv_pos;
//								glm::vec2 fv_delta;
//								// for mouse wheel
//								float wheel_delta;
//				}; 
//
//				LRESULT CALLBACK msg_callback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//
