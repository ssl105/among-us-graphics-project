#include "Transform.h"

Transform::Transform() {
    M = glm::mat4(1);
    ani = false;
    rAxis = glm::vec3(0, 1, 0);
}

Transform::~Transform() {
    deleteChild();
}

void Transform::draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, const glm::mat4& light, GLuint shader)
{
    for (int i = 0; i < children.size(); i++){
        Node * child = children[i];
        child->draw(C*M, view, projection, light, shader);
    }
}

void Transform::update(const glm::mat4& C) {
    if (ani) {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(0.1f), rAxis);

        M = M*rot;
    }

    //M = C * M;

    for (int i = 0; i < children.size(); i++){
        Node * child = children[i];
        child->update(C*M);
    }
    
}

void Transform::addChild(Node* child) {
    children.push_back(child);
}

void Transform::track(float deg, glm::vec3 rotAxi) {
    // rotate model
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(deg), rotAxi);
    
    M =  rot * M;
    
}

void Transform::updateSize(GLfloat size, glm::vec3 v)
{
    glm::vec3 scalV;
    
    if (v.x == 0) {
        scalV.x = 1;
    } else {
        scalV.x = size;
    }
    if (v.y == 0) {
        scalV.y = 1;
    } else {
        scalV.y = size;
    }
    if (v.z == 0) {
        scalV.z = 1;
    } else {
        scalV.z = size;
    }
    
    glm::mat4 scal = glm::scale(glm::mat4(1.0f), scalV);
    
    M = scal * M;
    
}

void Transform::changeCenter(glm::vec3 cen) {
    glm::mat4 t = glm::translate(glm::mat4(1.0f), cen);
    
    M = t * M;
    
}

void Transform::animate(glm::vec3 rotAxi) {
    ani = !ani;

    rAxis = rotAxi;
}

void Transform::deleteChild() {
    for (int i = 0; i < children.size(); i++) {
        delete children[i];
    }
}