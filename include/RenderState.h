#pragma once


struct RenderContext
{
		int width;		///< 窗口宽度
		int height;		///< 窗口高度
		int bpp;		///< 像素字节数(4byte)
		unsigned int* frameBuffer; ///< 像素数组首地址
		float* depthBuffer; ///< 深度缓冲首地址
};

