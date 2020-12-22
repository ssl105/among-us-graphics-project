#include "Geometry.h"

Geometry::Geometry(std::string objFilename, glm::mat3 color, float shine) : toon(false), texShade(false), cen(glm::vec3(0)), motion(false), mCount(0)
{
    srand((unsigned)time(NULL)); // rng

    setVis(true); // object visible at start
	
    ifstream objFile(objFilename); // The obj file we are reading.
    
    // Check whether the file can be opened.
    if (objFile.is_open()) {
        string line; // A line in the file.

        // Read lines from the file.
        while (getline(objFile, line)) {
            // Turn the line into a string stream for processing.
            stringstream ss;
            ss << line;

            // Read the first word of the line.
            std::string label;
            ss >> label;

            // If the line is about vertex (starting with a "v").
            if (label == "v") {
                // Read the later three float numbers and use them as the
                // coordinates.
                glm::vec3 point;
                ss >> point.x >> point.y >> point.z;

                // Process the point. For example, you can save it to a.
                input_points.push_back(point);
            }
            else if (label == "vn") {
                glm::vec3 normal;
                ss >> normal.x >> normal.y >> normal.z;

                input_normals.push_back(normal);
            }
            else if (label == "vt") {
                glm::vec2 text;
                ss >> text.x >> text.y;

                input_texts.push_back(text);
            }
            else if (label == "f") {
                unsigned int val;
                glm::ivec3 face;
                glm::ivec3 normalInd;
                glm::ivec3 textInd;
                
                // x coordinates
                ss >> val; // vertex indices
                face.x = val - 1;
                ss.get();
                char c;
                ss.get(c);
                if (c != '/') {
                    ss.unget();
                    ss >> val;
                    ss.get(c);

                    textInd.x = val - 1; // texture indices
                }
                ss >> val;  // vertex normal indices
                normalInd.x = val - 1;
                
                // y coordinates
                ss >> val;
                face.y = val - 1;
                ss.get();
                ss.get(c);
                if (c != '/') {
                    ss.unget();
                    ss >> val;
                    ss.get(c);

                    textInd.y = val - 1;
                }
                ss >> val;
                normalInd.y = val - 1;
                
                // y coordinates
                ss >> val;
                face.z = val - 1;
                ss.get();
                ss.get(c);
                if (c != '/') {
                    ss.unget();
                    ss >> val;
                    ss.get(c);

                    textInd.z = val - 1;
                }
                ss >> val;
                normalInd.z = val - 1;
                
                faces.push_back(face);
                normalInds.push_back(normalInd);
                textureInds.push_back(textInd);
            }
            
        }
    }
    else {
        cerr << "Can't open the file " << objFilename << endl;
    }
    
    objFile.close();


    // center the points
    // obtain maxes
    min.x = FLT_MAX;
    min.y = min.x;
    min.y = min.x;
    
    max.x = FLT_MIN;
    max.y = max.x;
    max.z = max.x;
    
    for (int i = 0; i < input_points.size(); i++) {
        glm::vec3 p = input_points[i];
        
        if (p.x < min.x) {
            min.x = p.x;
        }
        if (p.x > max.x) {
            max.x = p.x;
        }
        if (p.y < min.y) {
            min.y = p.y;
        }
        if (p.y > max.y) {
            max.y = p.y;
        }
        if (p.z < min.z) {
            min.z = p.z;
        }
        if (p.z > max.z) {
            max.z = p.z;
        }
    }
    
    glm::vec3 cen;
    cen.x = (min.x + max.x)/2;
    cen.y = (min.y + max.y)/2;
    cen.z = (min.z + max.z)/2;
    
    for (int i = 0; i < input_points.size(); i++) {
        glm::vec3 p = input_points[i];
        p.x = p.x - cen.x;
        p.y = p.y - cen.y;
        p.z = p.z - cen.z;
        
        input_points[i] = p;
    }
    

    // calculate radius for bounding sphere
     // obtain maxes
    r = FLT_MIN;

    for (int i = 0; i < input_points.size(); i++) {
        glm::vec3 p = input_points[i];
        
        if (p.x > r || p.z > r) {
            maxD = p;
            r = std::max(p.x, p.z);
        }
    }

    // set direction
    dir = glm::vec3(0, 0, 1);
    
    // reorder vertices
    for (unsigned int i = 0; i < faces.size(); i++) {
        
        points.push_back(input_points[faces[i].x]);
        normals.push_back(input_normals[normalInds[i].x]);
        textures.push_back(input_texts[textureInds[i].x]);
        
        points.push_back(input_points[faces[i].y]);
        normals.push_back(input_normals[normalInds[i].y]);
        textures.push_back(input_texts[textureInds[i].y]);
        
        points.push_back(input_points[faces[i].z]);
        normals.push_back(input_normals[normalInds[i].z]);
        textures.push_back(input_texts[textureInds[i].z]);
        
        indices.push_back(glm::vec3(i*3, i*3+1, i*3+2));
        
    }
       
    
	// Set the color. 
    mats.ambient = color[0];
    mats.diffuse = color[1];
    mats.specular = color[2];
    mats.shininess = shine;

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO1);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &VBO3);

	// Bind VAO
	glBindVertexArray(VAO);

    // vertices
	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
   
    // normals
    // Bind VBO to the bound VAO, and store the point data
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    // Enable Vertex Attribute 1 to pass point data through to the shader
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
   
	// textures
	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)* textures.size(), textures.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 2 to pass point data through to the shader
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    // vertex indices
    // Generate EBO, bind the EBO to the bound VAO, and send the index data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3)* indices.size(), indices.data(), GL_STATIC_DRAW);
    
	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Geometry::~Geometry()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &VBO3);
    glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Geometry::draw(const glm::mat4& C, const glm::mat4& view, const glm::mat4& projection, const glm::mat4& light, GLuint shader)
{
    model = C;

    // dont render invisible objects
    if (!vis) {
        return;
    }

	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(glGetUniformLocation(shader, "material.ambient"), 1, glm::value_ptr(mats.ambient));
    glUniform3fv(glGetUniformLocation(shader, "material.diffuse"), 1, glm::value_ptr(mats.diffuse));
    glUniform3fv(glGetUniformLocation(shader, "material.specular"), 1, glm::value_ptr(mats.specular));
    glUniform1f(glGetUniformLocation(shader, "material.shininess"), mats.shininess);
    glUniform3fv(glGetUniformLocation(shader, "dirLight.dir"), 1, glm::value_ptr(glm::vec3(light[0])));
    glUniform3fv(glGetUniformLocation(shader, "dirLight.col"), 1, glm::value_ptr(glm::vec3(light[1])));
    glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(glm::vec3(light[3])));
    glUniform1f(glGetUniformLocation(shader, "toon"), toon);
    glUniform1f(glGetUniformLocation(shader, "texShade"), texShade);
    glUniform1i(glGetUniformLocation(shader, "texture1"), 0);

	// Bind the VAO
	glBindVertexArray(VAO);

    // texture
    if (texShade) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texCode);
    }
    
	// Draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, sizeof(glm::ivec3)* indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Geometry::update(const glm::mat4& C)
{
    model = C;
}

void Geometry::loadTexture(string path)
{
    texShade = true;
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

     
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    texCode = textureID;
}


//getters 

float Geometry::getRadius() {
    glm::vec3 cMax = glm::vec3(model * glm::vec4(maxD, 1.0f));
    glm::vec3 cCen = glm::vec3(model * glm::vec4(cen, 1.0f));

    return glm::length(cMax - cCen);
}

glm::vec3 Geometry::getCen() {
    return glm::vec3(model * glm::vec4(cen, 1.0f));
}

glm::vec3 Geometry::getDir() {
    glm::vec3 cDir = glm::vec3(model * glm::vec4(dir, 1.0f));
    glm::vec3 cCen = glm::vec3(model * glm::vec4(cen, 1.0f));
    
    return glm::normalize(cDir - cCen);
}

bool Geometry::getMotion() {
    mCount--;
    return motion;
}

bool Geometry::getVis() {
    return vis;
}

bool Geometry::getSpawn() {
    bool s = recentlySpawned;
    if (s) {
        recentlySpawned = false;
    }

    return s;
}

int Geometry::getCount() {
    return mCount;
}


//setters
void Geometry::toonShading() {
    toon = !toon;
}

void Geometry::setSpawn(bool state) {
    recentlySpawned = state;
}

void Geometry::setVis(bool state) {
    vis = state;


    //stop movement whenever you change visibility
    setMotion(false);

    // recently spawned or despawned
    setSpawn(true);
}

void Geometry::setMotion(bool state) {
    motion = state;

    if (!motion) {
        mCount = rand() % 10000 + 5000;
    }
}