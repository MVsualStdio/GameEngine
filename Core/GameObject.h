#pragma once
#include <rttr/registration>
#include "Component.h"

class GameObject {
public:
    GameObject();
    GameObject(std::string name);
    ~GameObject();
    std::string& name();
    void setName(std::string name);
    Component* addComponent(std::string componentName);
    Component* getComponent(std::string componentName);
    std::vector<Component*>& getComponents(std::string componentName);
private:
    std::string m_name;
    std::unordered_map<std::string, std::vector<Component*>> m_componentInstance;
};