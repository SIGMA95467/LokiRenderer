#include "Texture.h"
#include "mathUtils.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::string& replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value)
{
    for (std::string::size_type pos(0); pos != std::string::npos; pos += new_value.length()) {
        if ((pos = str.find(old_value, pos)) != std::string::npos)
            str.replace(pos, old_value.length(), new_value);
        else   break;
    }
    return   str;
}

Texture::Texture(std::string path)
{
    mPath = replace_all_distinct(path, "\\", "/");
    
    //stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path.c_str(), &mWidth, &mHeight, &mChannels, 0);
    
    if (data) {
        mPixels = new unsigned char[mWidth * mHeight * mChannels];
        for (int i = 0; i < mWidth * mHeight * mChannels; ++i) 
            //mPixels[i] = std::pow((float)data[i] * (1 / 255.0f), 2.2f);
             mPixels[i] = data[i];

    }
    else {
        printf("Failed to load texture at: %s\n", path.c_str());
    }
    stbi_image_free(data);
}






float Texture::SampleIntensity(float u, float v) const
{
    //CLAMP
    mathUtils::Clamp(u);
    mathUtils::Clamp(v);

    float intU;
    float tU = std::modf(u * (mWidth - 1), &intU);
    int uIntLo = (int)intU;
    //向上取整
    int uIntHi = std::ceil(u * (mWidth - 1));

    float intV;
    float tV = std::modf(v * (mHeight - 1), &intV);
    int vIntLo = (int)intV;
    int vIntHi = std::ceil(v * (mHeight - 1));

    float IntensityRows[2];
    float IntensitySquare[4];

    GetIntensity(IntensitySquare[0], uIntLo, vIntLo);
    GetIntensity(IntensitySquare[1], uIntHi, vIntLo);
    GetIntensity(IntensitySquare[2], uIntLo, vIntHi);
    GetIntensity(IntensitySquare[3], uIntHi, vIntHi);

    
    IntensityRows[0] = mathUtils::Lerp(IntensitySquare[0], IntensitySquare[1], tU);
    IntensityRows[1] = mathUtils::Lerp(IntensitySquare[2], IntensitySquare[3], tU);
    float FinalIntensity;
    FinalIntensity = mathUtils::Lerp(IntensityRows[0], IntensityRows[1], tV);



    return FinalIntensity / 255.0f;
}

glm::vec3 Texture::SampleColor(float u, float v) const
{
    //CLAMP
    u = mathUtils::Clamp(u);
    v = mathUtils::Clamp(v);

    return BilinearSampleColor(u, v);
    //return NearestSampleColor(u, v);
}
//双线性采样
glm::vec3 Texture::BilinearSampleColor(float u, float v) const
{
    float intU;
    //取小数部分作为插值系数
    float tU = std::modf(u * (mWidth - 1), &intU);
    //向下取整
    int uIntLo = (int)intU;
    //向上取整
    int uIntHi = std::ceil(u * (mWidth - 1));

    float intV;
    float tV = std::modf(v * (mHeight - 1), &intV);
    int vIntLo = (int)intV;
    int vIntHi = std::ceil(v * (mHeight - 1));

    static glm::vec3 ColorRows[2];
    static glm::vec3 colorSquare[4];

    GetRGB(colorSquare[0], uIntLo, vIntLo);
    GetRGB(colorSquare[1], uIntHi, vIntLo);
    GetRGB(colorSquare[2], uIntLo, vIntHi);
    GetRGB(colorSquare[3], uIntHi, vIntHi);


    mathUtils::Lerp(ColorRows[0], colorSquare[0], colorSquare[1], tU);
    mathUtils::Lerp(ColorRows[1], colorSquare[2], colorSquare[3], tU);
    static glm::vec3 FinalColor;
    mathUtils::Lerp(FinalColor, ColorRows[0], ColorRows[1], tV);

    return FinalColor / 255.0f;
}
//临近点采样
glm::vec3 Texture::NearestSampleColor(float u, float v) const
{
    glm::vec3 FinalColor;
    int intU = std::round(u * (mWidth - 1));
    int intV = std::round(v * (mWidth - 1));
    GetRGB(FinalColor, intU, intV);

    return FinalColor / 255.0f;
    
}

void Texture::GetRGB(glm::vec3& col, int u, int v) const
{
    int index = (v * mWidth + u) * mChannels;
    col.z = mPixels[index];
    col.y = mPixels[index + 1];
    col.x = mPixels[index + 2];
}

void Texture::GetIntensity(float& intensity, int u, int v) const
{
    int index = (v * mWidth + u) * mChannels;
    intensity = mPixels[index];
}




int Texture::GetWidth()
{
    return mWidth;
}

int Texture::GetHeight()
{
    return mHeight;
}