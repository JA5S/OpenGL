#pragma once

#include "GLErrorManager.h"

#include <string>

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP; //Bytes per pixel
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
	
	inline int GetWidth() { return m_Width; }
	inline int GetHeight() { return m_Height; }
};

