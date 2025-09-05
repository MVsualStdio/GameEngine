#pragma once

#include "D3D11Context.h"
#include "Texture.h"
#include <unordered_map>
#include <memory>
#include "CBuffer.h"
#include "IDrawer.h"
#include "Shader.h"
#include "EffectType.h"


class Material {
public:

	Material(D3D11Context* context);
	~Material();
	void addTexture(std::string name, Texture2D texture);
	Texture2D getTexture(std::string name);
	void setVSShader(LPCWSTR filename, EffectType type = EffectType::None, LPCSTR entry = "VS");
	void setPSShader(LPCWSTR filename, EffectType type = EffectType::None, LPCSTR entry = "PS");
	std::shared_ptr<VSShader> getVSShader();
	std::shared_ptr<PSShader> getPSShader();

private:
	D3D11Context* m_context;
	std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_textures;

	std::shared_ptr<PSShader> m_PS;
	std::shared_ptr<VSShader> m_VS;
};