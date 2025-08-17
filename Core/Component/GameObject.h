#pragma once
#include "Component.h"
#include <string>
#include <vector>
#include <unordered_map>

class GameObject {
public:
    GameObject();
    GameObject(std::string name);
    virtual ~GameObject();
    std::string& name();
    void setName(std::string name);
    Component* addComponent(std::string componentName);
    Component* getComponent(std::string componentName);
    std::vector<Component*>& getComponents(std::string componentName);

    void setLayer(unsigned char layer) { m_layer = layer; }
    unsigned char getLayer() { return m_layer; }

    static std::list<GameObject*> getALLGameObject();

    virtual void update(double dt);

private:
    unsigned char m_layer;
    std::string m_name;
    std::unordered_map<std::string, std::vector<Component*>> m_componentInstance;
    static std::list<GameObject*> gGameobject;
};