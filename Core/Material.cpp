#include "Material.h"
#include <d3dcompiler.h>
#include <iostream>

Material::Material(D3D11Context* context)
	: m_context(context) {

}
Material::~Material() {

}
void Material::addTexture(std::string name, Texture2D texture) {
	if (m_textures.find(name) != m_textures.end()) {
		m_textures[name] = std::make_unique<Texture2D>(texture);
	}
	else {
		m_textures.emplace(name, std::make_unique<Texture2D>(texture));
	}
}

Texture2D Material::getTexture(std::string name) {
	if (m_textures.find(name) != m_textures.end()) {
		return *m_textures[name];
	}
	else {
		return Texture2D(m_context, 0, 0);
	}
}

void Material::setVSShader(LPCWSTR filename, EffectType type, LPCSTR entry) {
	m_VS = std::make_shared<VSShader>(m_context, filename, entry, type);
	
}

void Material::setPSShader(LPCWSTR filename, EffectType type, LPCSTR entry) {
	m_PS = std::make_shared<PSShader>(m_context, filename, entry, type);
}

std::shared_ptr<VSShader> Material::getVSShader() {
	return m_VS;
}

std::shared_ptr<PSShader> Material::getPSShader() {
	return m_PS;
}
