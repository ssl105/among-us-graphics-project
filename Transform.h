#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Node.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

#include <string>
#include <vector>

using namespace std;

class Transform : public Node
{
private:
    glm::mat4 M;
    vector<Node *>children;
    bool ani;
    glm::vec3 rAxis;
    
public:
    Transform();
    ~Transform();
    
    void draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, const glm::mat4& light, GLuint shader);
    void update(const glm::mat4& C);
    void addChild(Node* child);
    void track(float deg, glm::vec3 rotAxi);
    void updateSize(GLfloat size, glm::vec3 v);
    void changeCenter(glm::vec3 cen);
    void animate(glm::vec3 rotAxi);
    void deleteChild();

};

#endif

