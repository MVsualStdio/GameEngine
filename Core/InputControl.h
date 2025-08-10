#pragma once
#include <unordered_set>
/* Printable keys */
#define KEY_CODE_SPACE              32
#define KEY_CODE_APOSTROPHE         39  /* ' */
#define KEY_CODE_COMMA              44  /* , */
#define KEY_CODE_MINUS              45  /* - */
#define KEY_CODE_PERIOD             46  /* . */
#define KEY_CODE_SLASH              47  /* / */
#define KEY_CODE_0                  48
#define KEY_CODE_1                  49
#define KEY_CODE_2                  50
#define KEY_CODE_3                  51
#define KEY_CODE_4                  52
#define KEY_CODE_5                  53
#define KEY_CODE_6                  54
#define KEY_CODE_7                  55
#define KEY_CODE_8                  56
#define KEY_CODE_9                  57
#define KEY_CODE_SEMICOLON          59  /* ; */
#define KEY_CODE_EQUAL              61  /* = */
#define KEY_CODE_A                  65
#define KEY_CODE_B                  66
#define KEY_CODE_C                  67
#define KEY_CODE_D                  68
#define KEY_CODE_E                  69
#define KEY_CODE_F                  70
#define KEY_CODE_G                  71
#define KEY_CODE_H                  72
#define KEY_CODE_I                  73
#define KEY_CODE_J                  74
#define KEY_CODE_K                  75
#define KEY_CODE_L                  76
#define KEY_CODE_M                  77
#define KEY_CODE_N                  78
#define KEY_CODE_O                  79
#define KEY_CODE_P                  80
#define KEY_CODE_Q                  81
#define KEY_CODE_R                  82
#define KEY_CODE_S                  83
#define KEY_CODE_T                  84
#define KEY_CODE_U                  85
#define KEY_CODE_V                  86
#define KEY_CODE_W                  87
#define KEY_CODE_X                  88
#define KEY_CODE_Y                  89
#define KEY_CODE_Z                  90
#define KEY_CODE_LEFT_BRACKET       91  /* [ */
#define KEY_CODE_BACKSLASH          92  /* \ */
#define KEY_CODE_RIGHT_BRACKET      93  /* ] */
#define KEY_CODE_GRAVE_ACCENT       96  /* ` */
#define KEY_CODE_WORLD_1            161 /* non-US #1 */
#define KEY_CODE_WORLD_2            162 /* non-US #2 */

class InputControl {

public:
	static void keyPress(int key);
	static void mouseMove(float dx, float dy);
	static std::vector<float> dMouseMove();
	static bool hasKey(int key);
	static bool hasMouse();
	static void clear();
private:
	static std::unordered_set<int> gKeyPress;
	static std::vector<float> gMouseMove;
};