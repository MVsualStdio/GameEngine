#pragma once
#include <vector>

class GameObject;
class Component {
public:
	Component();
	virtual ~Component();
	GameObject* gameObject();
	void setGameObject(GameObject* object);
	virtual void update(double dt);

	static std::vector<Component*> getAllComponents();

private:
	GameObject* m_gameObject;
	static std::vector<Component*> gAllComponent;
};