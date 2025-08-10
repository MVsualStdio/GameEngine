#include "InputControl.h"

std::unordered_set<int> InputControl::gKeyPress;
std::vector<float> InputControl::gMouseMove;

void InputControl::keyPress(int key) {
	gKeyPress.emplace(key);
}

void InputControl::clear() {
	gKeyPress.clear();
	gMouseMove.clear();
}

bool InputControl::hasKey(int key) {
	return gKeyPress.find(key) != gKeyPress.end();
}

bool InputControl::hasMouse() {
	return gMouseMove.size() > 0;
}

void InputControl::mouseMove(float dx, float dy) {
	gMouseMove.push_back(dx);
	gMouseMove.push_back(dy);
}

std::vector<float> InputControl::dMouseMove() {
	return gMouseMove;
}