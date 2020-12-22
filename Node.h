#ifndef _NODE_H_
#define _NODE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <string>
//#include <vector>

class Node
{
public:
    virtual void draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, const glm::mat4& light, GLuint shader) = 0;
    virtual void update(const glm::mat4& C) = 0;
    
};

#endif

