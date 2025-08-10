#include "Component.h"

std::vector<Component*> Component::gAllComponent;

std::vector<Component*> Component::getAllComponents() {
	return gAllComponent;
}

Component::Component() {
	gAllComponent.push_back(this);
}

Component::~Component() {

}

GameObject* Component::gameObject() {
	return m_gameObject;
}

void Component::setGameObject(GameObject* object) {
	m_gameObject = object;
}

void Component::update(double dt) {

}