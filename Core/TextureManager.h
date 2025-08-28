#pragma once

#include <unordered_set>
#include <unordered_map>
#include "Texture.h"
#include "DDSTextureLoader11.h"

class TextureManager {
public:
	static TextureManager* instance();
	void addTexture(std::string path);
	
	Texture2D* getTexture(std::string path, D3D11Context* context);
	~TextureManager() = default;
private:
	std::unordered_map<std::string, std::shared_ptr<Texture2D>> gTextures;
	std::unordered_set<std::string> gLazyCache;

	TextureManager() = default;
	void loading(D3D11Context* context);
	std::string getFileExtension(const std::string& path);

	bool isDDS(std::string& suffix);
	bool isPNG(std::string& suffix);
};