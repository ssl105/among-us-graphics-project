#ifndef _WINDOW_H_
#define _WINDOW_H_
#define _USE_MATH_DEFINES

#include "main.h"
#include "shader.h"
#include "Node.h"
#include "Geometry.h"
#include "Transform.h"
#include "ParticleSystem.h"

#include <cmath>
#include <vector>
#include <time.h>

class Window
{
public:
    
    class PointLight {
    public:
        glm::vec3 pos, color, atten;
        
    };

    class DirLight {
    public:
        glm::vec3 dir, color;
    };

    class Plane {
    public:
        glm::vec3 point, norm;
    };
    
    // light
    static PointLight light;
    static DirLight dLight;

	// Window Properties
	static int width;
	static int height;
    static int w, h;
	static const char* windowTitle;

	// Objects to Render
    static Geometry * curr;
    static Transform* world;
    static ParticleSystem* particleEffect;

	// Camera Matrices
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eyePos, lookAtPoint, upVector;

	// Shader Program ID
	static GLuint shaderProgram;
    static GLuint particleShader;

	// Constructors and Destructors
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();

	// Window functions
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// Draw and Update functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    
    // movement
    enum MovementType { ROTATE, NONE };
    static MovementType Movement;
    
    // light control mode
    static int mode;

    //collision detection algorithms
    static bool sphereCollision(Geometry* s1, Geometry* s2);
    static bool spherePlaneCollision(Geometry* s1, Window::Plane p);
    
};

#endif
