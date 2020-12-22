#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include "Node.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

#include <vector>
#include <string>
#include <time.h>

using namespace std;

class ParticleSystem
{
private:
    struct Particle {
        glm::vec3 position;
        float life;

        void intialize(bool spawn);

        void update(bool spawn);

    };

    static const int MAX_PARTICLES = 200;
    static const int PARTICLE_LIFE = 5000;

    GLuint VAO, VBO;

    vector <Particle> particles;
    vector <glm::vec3> position_data;

    glm::vec3 source, color;
    bool vis;


public:
    ParticleSystem();
    ~ParticleSystem();

    void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
    void update(bool spawn);
    void reset(bool spawn);
    void setSource(glm::vec3 cen);
    void setColor(glm::mat3 col);
    void setVis(bool state);
};

#endif
