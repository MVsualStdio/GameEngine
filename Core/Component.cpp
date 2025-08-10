#include "Component.h"

Component::Component() {

}

Component::~Component() {

}

GameObject* Component::gameObject() {
	return m_gameObject;
}

void Component::setGameObject(GameObject* object) {
	m_gameObject = object;
}