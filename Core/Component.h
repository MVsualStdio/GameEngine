#pragma once

class GameObject;
class Component {
public:
	Component();
	virtual ~Component();
	GameObject* gameObject();
	void setGameObject(GameObject* object);
private:
	GameObject* m_gameObject;

};