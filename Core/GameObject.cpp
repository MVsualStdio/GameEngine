#include "GameObject.h"

GameObject::GameObject() {

}

GameObject::GameObject(std::string name) 
	: m_name(name) {

}

GameObject::~GameObject() {

}

std::string& GameObject::name() { 
	return m_name; 
}

void GameObject::setName(std::string name) {
	m_name = name;
}

Component* GameObject::addComponent(std::string componentName) {
	rttr::type t = rttr::type::get_by_name(componentName);
	rttr::variant var = t.create();
	Component* component = var.get_value<Component*>();
	component->setGameObject(this);
	if (m_componentInstance.find(componentName) == m_componentInstance.end()) {
		std::vector<Component*> componentVec;
		componentVec.push_back(component);
		m_componentInstance[componentName] = componentVec;
	}
	else {
		m_componentInstance[componentName].push_back(component);
	}
	return component;
}

Component* GameObject::getComponent(std::string componentName) {
	if (m_componentInstance.find(componentName) == m_componentInstance.end()) {
		return nullptr;
	}
	if (m_componentInstance[componentName].size() == 0) {
		return nullptr;
	}
	return m_componentInstance[componentName][0];
}

std::vector<Component*>& GameObject::getComponents(std::string componentName) {
	return m_componentInstance[componentName];

}