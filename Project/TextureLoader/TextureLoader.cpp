#include "TextureLoader.h"

TextureLoader* TextureLoader::GetInstance(){
    static TextureLoader textureLoader;


    return &textureLoader;
}

void TextureLoader::Initialize()
{
}
