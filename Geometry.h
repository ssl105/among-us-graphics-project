#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "Node.h"
#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

#include <vector>
#include <string>
#include <time.h>

using namespace std;

class Geometry : public Node
{
public:
    class Material {
    public:
        glm::vec3 ambient, diffuse, specular;
        float shininess;
    };
    
private:
    glm::mat4 model;
    bool toon;
    bool texShade;
    bool vis;
    bool motion;
    bool recentlySpawned;
    GLuint texCode;
	vector<glm::vec3> points;
    vector<glm::vec3> normals;
    vector<glm::vec2> textures;
    vector<glm::vec3> input_points;
    vector<glm::vec3> input_normals;
    vector<glm::vec2> input_texts;
    vector<glm::vec3> textureInds;
    vector<glm::ivec3> normalInds;
    vector<glm::ivec3> faces;
    vector<glm::ivec3> indices;
    Material mats;
    glm::vec3 min, max, cen, maxD, dir;
    float r;
    int mCount;

    GLuint VAO, VBO1, VBO2, VBO3, EBO;
    
public:
    
	Geometry(std::string objFilename, glm::mat3 color, float shine);
    ~Geometry();
	
	void draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, const glm::mat4& light, GLuint shader);
	void update(const glm::mat4& C);
    void loadTexture(string path);

    //getters
    float getRadius();
    bool getVis();
    bool getMotion();
    bool getSpawn();
    int getCount();
    glm::vec3 getCen();
    glm::vec3 getDir();

    //setters
    void setVis(bool state);
    void setSpawn(bool state);
    void setMotion(bool state);
    void toonShading();


    
};

#endif
