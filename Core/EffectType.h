#pragma once 

enum class EffectType : uint32_t {
	None = 0,
	ShadowMap = 1 << 0,

};

inline EffectType operator|(EffectType a, EffectType b) {
	return static_cast<EffectType>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline EffectType operator&(EffectType a, EffectType b) {
	return static_cast<EffectType>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

inline bool HasEffect(EffectType value, EffectType flag) {
	return (static_cast<uint32_t>(value) & static_cast<uint32_t>(flag)) != 0; \
}