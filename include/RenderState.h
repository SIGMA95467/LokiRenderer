#pragma once


struct RenderContext
{
		int width;		///< ���ڿ��
		int height;		///< ���ڸ߶�
		int bpp;		///< �����ֽ���(4byte)
		unsigned int* frameBuffer; ///< ���������׵�ַ
		float* depthBuffer; ///< ��Ȼ����׵�ַ
};

