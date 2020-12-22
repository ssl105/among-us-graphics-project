#include "Window.h"

// Window Properties
int Window::width;
int Window::height;
int Window::w;
int Window::h;
const char* Window::windowTitle = "GLFW Starter Project";

// Objects to Render
Geometry * Window::curr;
Geometry* other;
Transform* currT;
Transform* otherT;
Transform* Window::world;
Transform* lobby;
ParticleSystem * Window::particleEffect;

//players
Transform* red;
Transform* purple;
Transform* blue;
Transform* black;
Transform* sky;
Transform* white;
Transform* pink;
Transform* orange;
Transform* yellow;
Transform* lgreen;
Transform* dgreen;
Transform* brown;
vector<Geometry*> astros;
vector<Transform*> astrosT;
vector<glm::mat3> colors;
int lobbySize = 10;
int SPAWN = 1;
int DESPAWN = 0;
int state = -1;

//bounding 
Transform* rBox;
Transform* lBox;
Geometry* box;
Geometry* rAstro;

//list of bounding spheres
vector<Geometry*> bounds;

//planes
Window::Plane t, b, r, l, blc, brc, cPlane;
vector<Window::Plane> planes;

// movement spheres
glm::vec3 cen;

// movement constants
float step = 0.002;
float scale = 1.1;
vector<glm::vec3> moves;
int UP = 0;
int DOWN = 1;
int LEFT = 2;
int RIGHT = 3;
float rotAng = 0;
glm::vec3 dir, rotAxi;


// Camera Matrices 
// Projection matrix:
glm::mat4 Window::projection; 

// View Matrix:
glm::vec3 Window::eyePos(0, 0, 50);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);
glm::vec3 lastPos;

//camera rotation
float lastX;
float lastY;
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 60.0;
static const float m_ROTSCALE = 90.0;

// camera movement mode
Window::MovementType Window::Movement = NONE;

// Shader Program ID
GLuint Window::shaderProgram;
GLuint Window::particleShader;

//light
Window::DirLight Window::dLight;
int Window::mode = 1;

//texture
GLuint texture = 0;

// random iterator count
int rCount = 500;


bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    particleShader = LoadShaders("shaders/particleShader.vert", "shaders/particleShader.frag");
    
    // Check the shader program.
    if (!shaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
        
    }
    if (!particleShader) {
        std::cerr << "Failed to initialize particle shader program" << std::endl;
        return false;

    }
    
    //set up lights
    dLight.dir = glm::vec3(0, -20, -20);
    dLight.color = glm::vec3(1.0f, 1.0f , 0.9f);
    //light.atten = glm::vec3(0.0f, 1.0f, 0.0f);


    //set up planes bounds
    t.point = glm::vec3(0, 0, 0);
    t.norm = glm::vec3(0, 0, 1);
    //bot
    b.point = glm::vec3(0, 0, 16.75);
    b.norm = glm::vec3(0, 0, -1);
    //right
    r.point = glm::vec3(17, 0, 0);
    r.norm = glm::vec3(-1, 0, 0);
    //left
    l.point = glm::vec3(-16.125, 0, 0);
    l.norm = glm::vec3(1, 0, 0);
    //bottom left corner
    blc.point = glm::vec3(-12, 0, 16);
    blc.norm = glm::normalize(glm::vec3(1, 0, -1));
    //bottom right corner
    brc.point = glm::vec3(16, 0, 13);
    brc.norm = glm::normalize(glm::vec3(-1, 0, -1));

    planes.push_back(t);
    planes.push_back(b);
    planes.push_back(r);
    planes.push_back(l);
    planes.push_back(blc);
    planes.push_back(brc);

    moves.push_back(glm::vec3(0, 0, -1));
    moves.push_back(glm::vec3(0, 0, 1));
    moves.push_back(glm::vec3(-1, 0, 0));
    moves.push_back(glm::vec3(1, 0, 0));

    return true;
}

bool Window::initializeObjects()
{
    // random number generator
    srand(glfwGetTime());

    //colors
	glm::mat3 color1, cRed, cPurple, cBlue, cBlack, cSky, cWhite, cPink, cOrange, cYellow, cLgreen, cDgreen, cBrown;
    //default color
	color1[0] = glm::vec3(0.2f, 0.2f, 0.2f);
	color1[1] = glm::vec3(0.3f, 0.3f, 0.3f);
	color1[2] = glm::vec3(0.02f, 0.01f, 0.01f);
    //red
    cRed[0] = glm::vec3(197.0f/255.0f, 18.0f/255.0f, 17.0f/255.0f);
    cRed[1] = glm::vec3(1.0f, 0.8f, 0.9f);
    cRed[2] = glm::vec3(0.7f, 0.6f, 0.9f);
    //purple
    cPurple[0] = glm::vec3(108.0f / 255.0f, 47.0f / 255.0f, 188.0f / 255.0f);
    cPurple[1] = glm::vec3(1.0f, 0.8f, 0.9f);
    cPurple[2] = glm::vec3(0.7f, 0.6f, 0.9f);
    //blue
    cBlue[0] = glm::vec3(19.0f / 255.0f, 46.0f / 255.0f, 209.0f / 255.0f);
    cBlue[1] = glm::vec3(1.0f, 0.8f, 0.9f);
    cBlue[2] = glm::vec3(0.7f, 0.6f, 0.9f);
    //black
    cBlack[0] = glm::vec3(62.0f / 255.0f, 71.0f / 255.0f, 78.0f / 255.0f);
    cBlack[1] = glm::vec3(1.0f, 0.8f, 0.9f);
    cBlack[2] = glm::vec3(0.7f, 0.6f, 0.9f);
    //sky
    cSky[0] = glm::vec3(57.0f / 255.0f, 254.0f / 255.0f, 221.0f / 255.0f);
    cSky[1] = glm::vec3(1.0f, 0.8f, 0.9f);
    cSky[2] = glm::vec3(0.7f, 0.6f, 0.9f);
    //white
    cWhite[0] = glm::vec3(214.0f / 255.0f, 223.0f / 255.0f, 241.0f / 255.0f);
    cWhite[1] = glm::vec3(1.0f, 0.8f, 0.9f);
    cWhite[2] = glm::vec3(0.7f, 0.6f, 0.9f);
    //pink
    cPink[0] = glm::vec3(236.0f / 255.0f, 84.0f / 255.0f, 187.0f / 255.0f);
    cPink[1] = glm::vec3(1.0f, 0.8f, 0.9f);
    cPink[2] = glm::vec3(0.7f, 0.6f, 0.9f);
    //orange
    cOrange[0] = glm::vec3(241.0f / 255.0f, 125.0f / 255.0f, 12.0f / 255.0f);
    cOrange[1] = glm::vec3(1.0f, 0.8f, 0.9f);
    cOrange[2] = glm::vec3(0.7f, 0.6f, 0.9f);
    //yellow
    cYellow[0] = glm::vec3(246.0f / 255.0f, 246.0f / 255.0f, 87.0f / 255.0f);
    cYellow[1] = glm::vec3(1.0f, 0.8f, 0.9f);
    cYellow[2] = glm::vec3(0.7f, 0.6f, 0.9f);
    //lgreen
    cLgreen[0] = glm::vec3(78.0f / 255.0f, 239.0f / 255.0f, 56.0f / 255.0f);
    cLgreen[1] = glm::vec3(1.0f, 0.8f, 0.9f);
    cLgreen[2] = glm::vec3(0.7f, 0.6f, 0.9f);
    //dgreen
    cDgreen[0] = glm::vec3(19.0f / 255.0f, 128.0f / 255.0f, 44.0f / 255.0f);
    cDgreen[1] = glm::vec3(1.0f, 0.8f, 0.9f);
    cDgreen[2] = glm::vec3(0.7f, 0.6f, 0.9f);
    //brown
    cBrown[0] = glm::vec3(113.0f / 255.0f, 73.0f / 255.0f, 29.0f / 255.0f);
    cBrown[1] = glm::vec3(1.0f, 0.8f, 0.9f);
    cBrown[2] = glm::vec3(0.7f, 0.6f, 0.9f);


    //lobby
    lobby = new Transform();
    lobby->addChild(curr = new Geometry("obj/lobby/amongus_lobby.obj", color1, 128*1.0f));
    curr->loadTexture("obj/lobby/amongus_lobby.png");


    //players
    //default player red
    red = new Transform();
    red->addChild(curr = new Geometry("obj/astro/amongus_astro_still.obj", cRed, 1.0f));
    rAstro = curr;
    red->changeCenter(glm::vec3(-0, -4, 2));
    curr->toonShading();
    astros.push_back(curr);
    astrosT.push_back(red);
    colors.push_back(cRed);

    //default player blue
    blue = new Transform();
    blue->addChild(curr = new Geometry("obj/astro/amongus_astro_still.obj", cBlue, 1.0f));
    astros.push_back(curr);
    curr->toonShading();
    blue->track(rand() % 360, glm::vec3(0, 1, 0));
    blue->changeCenter(glm::vec3(0, -4, 6));
    astrosT.push_back(blue);
    colors.push_back(cBlue);

    //default player black
    black = new Transform();
    black->addChild(curr = new Geometry("obj/astro/amongus_astro_still.obj", cBlack, 1.0f));
    astros.push_back(curr);
    curr->toonShading();
    black->track(rand() % 360, glm::vec3(0, 1, 0));
    black->changeCenter(glm::vec3(0, -4, 10));
    astrosT.push_back(black);
    colors.push_back(cBlack);

    //default player purple
    purple = new Transform();
    purple->addChild(curr = new Geometry("obj/astro/amongus_astro_still.obj", cPurple, 1.0f));
    astros.push_back(curr);
    curr->toonShading();
    purple->track(rand() % 360, glm::vec3(0, 1, 0));
    purple->changeCenter(glm::vec3(0, -4, 14));
    astrosT.push_back(purple);
    colors.push_back(cPurple);

    //default player sky
    sky = new Transform();
    sky->addChild(curr = new Geometry("obj/astro/amongus_astro_still.obj", cSky, 1.0f));
    curr->toonShading();
    curr->setVis(false);   // not visible at start
    astros.push_back(curr);
    sky->track(rand() % 360, glm::vec3(0, 1, 0));
    sky->changeCenter(glm::vec3(2, -4, 2));
    astrosT.push_back(sky);
    colors.push_back(cSky);

    //default player white
    white = new Transform();
    white->addChild(curr = new Geometry("obj/astro/amongus_astro_still.obj", cWhite, 1.0f));
    curr->toonShading();
    astros.push_back(curr);
    white->track(rand() % 360, glm::vec3(0, 1, 0));
    white->changeCenter(glm::vec3(2, -4, 6));
    astrosT.push_back(white);
    colors.push_back(cWhite);

    //default player pink
    pink = new Transform();
    pink->addChild(curr = new Geometry("obj/astro/amongus_astro_still.obj", cPink, 1.0f));
    curr->toonShading();
    astros.push_back(curr);
    pink->track(rand() % 360, glm::vec3(0, 1, 0));
    pink->changeCenter(glm::vec3(2, -4, 10));
    astrosT.push_back(pink);
    colors.push_back(cPink);

    //default player orange
    orange = new Transform();
    orange->addChild(curr = new Geometry("obj/astro/amongus_astro_still.obj", cOrange, 1.0f));
    curr->toonShading();
    astros.push_back(curr);
    orange->track(rand() % 360, glm::vec3(0, 1, 0));
    orange->changeCenter(glm::vec3(2, -4, 14));
    astrosT.push_back(orange);
    colors.push_back(cOrange);

    //default player yellow
    yellow = new Transform();
    yellow->addChild(curr = new Geometry("obj/astro/amongus_astro_still.obj", cYellow, 1.0f));
    curr->toonShading();
    curr->setVis(false); // not visible at start
    astros.push_back(curr);
    yellow->track(rand() % 360, glm::vec3(0, 1, 0));
    yellow->changeCenter(glm::vec3(-2, -4, 2));
    astrosT.push_back(yellow);
    colors.push_back(cYellow);

    //default player lgreen
    lgreen = new Transform();
    lgreen->addChild(curr = new Geometry("obj/astro/amongus_astro_still.obj", cLgreen, 1.0f));
    curr->toonShading();
    astros.push_back(curr);
    lgreen->track(rand() % 360, glm::vec3(0, 1, 0));
    lgreen->changeCenter(glm::vec3(-2, -4, 6));
    astrosT.push_back(lgreen);
    colors.push_back(cLgreen);

    //default player dgreen
    dgreen = new Transform();
    dgreen->addChild(curr = new Geometry("obj/astro/amongus_astro_still.obj", cDgreen, 1.0f));
    curr->toonShading();
    astros.push_back(curr);
    dgreen->track(rand() % 360, glm::vec3(0, 1, 0));
    dgreen->changeCenter(glm::vec3(-2, -4, 10));
    astrosT.push_back(dgreen);
    colors.push_back(cDgreen);

    //default player brown
    brown = new Transform();
    brown->addChild(curr = new Geometry("obj/astro/amongus_astro_still.obj", cBrown, 1.0f));
    curr->toonShading();
    astros.push_back(curr);
    brown->track(rand() % 360, glm::vec3(0, 1, 0));
    brown->changeCenter(glm::vec3(-2, -4, 14));
    astrosT.push_back(brown);
    colors.push_back(cBrown);

    //invisible box bounds
    rBox = new Transform();
    rBox->addChild(box = new Geometry("obj/trisphere.obj", color1, 128 * 1.0f));
    bounds.push_back(box);
    rBox->updateSize(1.33f, glm::vec3(1, 1, 1));
    rBox->changeCenter(glm::vec3(11.125, -4, 3.625));
    rBox->update(glm::mat4(1));

    lBox = new Transform();
    lBox->addChild(box = new Geometry("obj/trisphere.obj", color1, 128 * 1.0f));
    bounds.push_back(box);
    lBox->updateSize(1.33f, glm::vec3(1, 1, 1));
    lBox->changeCenter(glm::vec3(-9.125, -4, 7));
    lBox->update(glm::mat4(1));
    

    //base level of scene graph
    world = new Transform();
    world->addChild(lobby);
    world->addChild(red);
    world->addChild(blue);
    world->addChild(black);
    world->addChild(purple);
    world->addChild(sky);
    world->addChild(white);
    world->addChild(pink);
    world->addChild(orange);
    world->addChild(yellow);
    world->addChild(lgreen);
    world->addChild(dgreen);
    world->addChild(brown);

    // initialize particle effects
    particleEffect = new ParticleSystem();
    

    return true;
}

void Window::cleanUp()
{
    // Deallcoate the objects
    delete world;
    delete rBox;
    delete lBox;
    delete particleEffect;
  
    // Delete the shader program.
    glDeleteProgram(shaderProgram);
    glDeleteProgram(particleShader);
}

GLFWwindow* Window::createWindow(int width, int height)
{
    // Initialize GLFW.
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }
    
    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);
    
#ifdef __APPLE__ 
    // Apple implements its own version of OpenGL and requires special treatments
    // to make it uses modern OpenGL.
    
    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
    
    // Check if the window could not be created.
    if (!window)
    {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }
    
    // Make the context of the window.
    glfwMakeContextCurrent(window);
    
#ifndef __APPLE__
    // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.
    
    // Initialize GLEW.
    if (glewInit())
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return NULL;
    }
#endif
    
    // Set swap interval to 1.
    glfwSwapInterval(0);
    
    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);
    
    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
    
    Window::w = width;
    Window::h = height;
    
    //lastX = width/2;
    //lastY = height/2;
    
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
    
#endif
    Window::width = width;
    Window::height = height;
    
    // Set the viewport size.
    glViewport(0, 0, width, height);
    
    // Set the projection matrix.
    Window::projection = glm::perspective(glm::radians(fov),
                                          (float) width / (float)height, 1.0f, 1000.0f);
}

void Window::idleCallback()
{
    // Perform any necessary updates here
    
    // randomize spawning and despawning
    if (rCount == 0) {
        state = -1;
        int astroIndex = 0;

        // spawn
        for (int i = 1; i < astros.size(); i++) {
            curr = astros[i];
            currT = astrosT[i];

            if (!curr->getVis()) {
                if (rand()%7 < 2) {
                    if (lobbySize < 10) {
                        curr->setVis(true);

                        // rotate in random direction
                        currT->update(glm::mat4(1));
                        cen = curr->getCen();
                        currT->changeCenter(-cen);
                        currT->track(rand() % 360, glm::vec3(0, 1, 0));
                        currT->changeCenter(cen);
                        currT->update(glm::mat4(1));

                        lobbySize++;
                        state = SPAWN;
                        astroIndex = i;
                        break;
                    }
                }
            }
        }


        // despawn
        if (state == -1) {
            // randomize invisible chars
            int randNum1;
            randNum1 = (rand() % (astros.size() - 1)) + 1;


            // despawn astronauts
            if (rand() % 4 < 3) {
                if (astros[randNum1]->getVis()) {
                    astros[randNum1]->setVis(false);
                    lobbySize--;
                    state = DESPAWN;
                    astroIndex = randNum1;
                }
            }
        }

        // reset invis count
        rCount = rand() % 5000 + 7500;

        //create particle effect for spawns and despawns
        if (state != -1) {
            //reset particle system
            astrosT[astroIndex]->update(glm::mat4(1));
            particleEffect->setColor(colors[astroIndex]);
            particleEffect->setSource(astros[astroIndex]->getCen());
            particleEffect->reset(state);
        }

    }
    else {
        rCount--;

        //update particle system
        particleEffect->update(state);

    }


    //randomize movement stoppage
    for (int i = 0; i < astros.size(); i++) {
        curr = astros[i];

        // make sure player is visible
        if (!curr->getVis()) {
            continue;
        }

        if (curr->getMotion()) {
            // low chance to stop
            if (rand() % 100000 < 1) {
                curr->setMotion(false);
            }
        }
        else {
            if (curr->getCount() < 0) {
                curr->setMotion(true);
            }
        }
    }
    
    
    // move visible npc astronauts
    for (int i = 1; i < astros.size(); i++) {
        curr = astros[i];
        currT = astrosT[i];

        // check if astronaut is visible
        if (!curr->getVis()) {
            continue;
        }


        // check if astronaut is in motion
        if (curr->getMotion()) {
            // move astronaut in current direction foward
            currT->update(glm::mat4(1));
            dir = curr->getDir();

            currT->changeCenter(step * dir);
            currT->update(glm::mat4(1));

        }
    }


    // collision resolution
    for (int i = 0; i < astros.size(); i++) {
        curr = astros[i];
        currT = astrosT[i];


        // check if astronaut is visible
        if (!curr->getVis()) {
            continue;
        }


        // check astronauts
        for (int j = 0; j < astros.size(); j++) {
            other = astros[j];
            otherT = astrosT[j];

            // dont check same astronaut or Invisible astronauts.
            if (i == j || !other->getVis()) {
                continue;
            }

            // resolve player-player collision
            if (sphereCollision(curr, other)) {
                //break spheres apart from each other
                //update transformations before accessing vars
                currT->update(glm::mat4(1));
                otherT->update(glm::mat4(1));

                // initialize center, norm, and radius vars
                glm::vec3 c1, c2, n1, n2;
                float r1, r2;
                c1 = curr->getCen();
                c2 = other->getCen();
                r1 = curr->getRadius();
                r2 = other->getRadius();
                n1 = glm::normalize(c1 - c2);


                // if players are on top of each other, choose a random normal vector
                if (glm::length(n1) == 0) {
                    n1.x = rand() % 10;
                    n1.y = rand() % 10;
                    n1.z = rand() % 10;

                    n1 = glm::normalize(n1);
                }

                // len to push sphere
                float pushLen = (r1 + r2) - glm::length(c1 - c2) + 0.01f;

                currT->changeCenter(pushLen * n1);
                currT->update(glm::mat4(1));

                //change rotations for curr npc in motion
                if (i != 0 && curr->getMotion()) {
                    c1 = curr->getCen(); // updated center
                    dir = curr->getDir(); // curr direction
        
                    //rotate direction to match reflection 
                    glm::vec3 reflect = dir - 2 * (dot(dir, n1)) * n1; // reflection vector
                    rotAng = acos(dot(dir, reflect)) * 180.0 / M_PI;
                    rotAxi = cross(dir, reflect);
                    if (rotAxi == glm::vec3(0) || round(rotAng) == 180) {
                        rotAxi = glm::vec3(0, 1, 0);
                    }

                    currT->changeCenter(-c1);
                    currT->track(rotAng, rotAxi);
                    currT->changeCenter(c1);
                    currT->update(glm::mat4(1));
                }

                //change rotations for other npc in motion
                if (j != 0 && other->getMotion()) {
                    otherT->update(glm::mat4(1));
                    c2 = other->getCen(); // updated center
                    dir = other->getDir(); // curr direction
                    n2 = glm::normalize(c2-c1);

                    //rotate direction to match reflection 
                    glm::vec3 reflect = dir - 2 * (dot(dir, n2)) * n2; // reflection vector
                    rotAng = acos(dot(dir, reflect)) * 180.0 / M_PI;
                    rotAxi = cross(dir, reflect);
                    if (rotAxi == glm::vec3(0) || round(rotAng) == 180) {
                        rotAxi = glm::vec3(0, 1, 0);
                    }

                    otherT->changeCenter(-c2);
                    otherT->track(rotAng, rotAxi);
                    otherT->changeCenter(c2);
                    otherT->update(glm::mat4(1));
                }
            }

        }


        // check obstacles
        for (int j = 0; j < bounds.size(); j++) {
            other = bounds[j];

            // resolve player-sphere collision
            if (sphereCollision(curr, other)) {
                //break spheres apart from each other
                //update transformations before accessing vars
                currT->update(glm::mat4(1));

                // initialize center, norm, and radius vars
                glm::vec3 c1, c2, n1, n2;
                float r1, r2;
                c1 = curr->getCen();
                c2 = other->getCen();
                r1 = curr->getRadius();
                r2 = other->getRadius();
                n1 = (c1 - c2) / (glm::length(c1 - c2));
                n2 = -n1;

                // len to push sphere
                float pushLen = (r1 + r2) - glm::length(c1 - c2) + 0.01f;

                currT->changeCenter(pushLen * n1);

                //change rotations for curr npc in motion
                if (i != 0 && curr->getMotion()) {
                    currT->update(glm::mat4(1));
                    c1 = curr->getCen(); // updated center
                    dir = curr->getDir(); // curr direction

                    //rotate direction to match reflection 
                    glm::vec3 reflect = dir - 2 * (dot(dir, n1)) * n1; // reflection vector
                    rotAng = acos(dot(dir, reflect)) * 180.0 / M_PI;
                    rotAxi = cross(dir, reflect);
                    if (rotAxi == glm::vec3(0) || rotAng == 180) {
                        rotAxi = glm::vec3(0, 1, 0);
                    }

                    currT->changeCenter(-c1);
                    currT->track(rotAng, rotAxi);
                    currT->changeCenter(c1);
                    currT->update(glm::mat4(1));
                }
            }

        }


        // check planes
        for (int j = 0; j < planes.size(); j++) {
            cPlane = planes[j];

            // resolve player-plane collision
            if (spherePlaneCollision(curr, cPlane)) {
                //break spheres apart from each other
                //update transformations before accessing vars
                currT->update(glm::mat4(1));

                // initialize center and radius
                glm::vec3 c1;
                float r1;
                c1 = curr->getCen();
                r1 = curr->getRadius();
                float dist = abs(glm::dot((c1 - cPlane.point), cPlane.norm));   // dist from cen to plane

                // len to push sphere
                float pushLen = r1 - dist + 0.01f;

                currT->changeCenter(pushLen * cPlane.norm);

                //change rotations for curr npc in motion
                if (i != 0 && curr->getMotion()) {
                    currT->update(glm::mat4(1));
                    c1 = curr->getCen(); // updated center
                    dir = curr->getDir(); // curr direction

                    //rotate direction to match reflection 
                    glm::vec3 reflect = glm::normalize(dir - 2 * (dot(dir, cPlane.norm)) * cPlane.norm); // reflection vector
                    rotAng = acos(dot(dir, reflect)) * 180.0 / M_PI;
                    rotAxi = cross(dir, reflect);
                    if (rotAxi == glm::vec3(0) || rotAng == 180) {
                        rotAxi = glm::vec3(0, 1, 0);
                    }

                    currT->changeCenter(-c1);
                    currT->track(rotAng, rotAxi);
                    currT->changeCenter(c1);
                    currT->update(glm::mat4(1));
                }
            }
        }


    }

    
}

void Window::displayCallback(GLFWwindow* window)
{	
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // creating lighting matrix
    glm::mat4 lighting;
    lighting[0] = glm::vec4(dLight.dir,0);
    lighting[1] = glm::vec4(dLight.color,0);
    lighting[2] = glm::vec4(glm::vec3(0),0);
    lighting[3] = glm::vec4(eyePos,0);

    //render objects
    world->draw(glm::mat4(1), view, projection, lighting, shaderProgram);

    //render particle system
    particleEffect->draw(view, projection, particleShader);
    
    // Gets events, including input such as keyboard and mouse or window resizing
    glfwPollEvents();
    
    // Swap buffers.
    glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key) {
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;

		case GLFW_KEY_W:
            dir = rAstro->getDir();
            rotAng = acos(dot(dir, moves[UP])) * 180.0 / M_PI;
            rotAxi = cross(dir, moves[UP]);
            if (rotAxi == glm::vec3(0)) {
                rotAxi = glm::vec3(0, 1, 0);
            }
            cen = rAstro->getCen();
            red->changeCenter(-cen);
            red->track(rotAng, rotAxi);
            red->changeCenter(cen);
			red->changeCenter(1.0f * moves[UP]);
			break;
		case GLFW_KEY_S:
            dir = rAstro->getDir();
            rotAng = acos(dot(dir, moves[DOWN])) * 180.0 / M_PI;
            rotAxi = cross(dir, moves[DOWN]);
            if (rotAxi == glm::vec3(0)) {
                rotAxi = glm::vec3(0, 1, 0);
            }
            cen = rAstro->getCen();
            red->changeCenter(-cen);
            red->track(rotAng, rotAxi);
            red->changeCenter(cen);
			red->changeCenter(1.0f * moves[DOWN]);
			break;
		case GLFW_KEY_A:
            dir = rAstro->getDir();
            rotAng = acos(dot(dir, moves[LEFT])) * 180.0 / M_PI;
            rotAxi = cross(dir, moves[LEFT]);
            if (rotAxi == glm::vec3(0)) {
                rotAxi = glm::vec3(0, 1, 0);
            }
            cen = rAstro->getCen();
            red->changeCenter(-cen);
            red->track(rotAng, rotAxi);
            red->changeCenter(cen);
			red->changeCenter(1.0f* moves[LEFT]);
			break;
		case GLFW_KEY_D:
            dir = rAstro->getDir();
            rotAng = acos(dot(dir, moves[RIGHT])) * 180.0 / M_PI;
            rotAxi = cross(dir, moves[RIGHT]);
            if (rotAxi == glm::vec3(0)) {
                rotAxi = glm::vec3(0, 1, 0);
            }
            cen = rAstro->getCen();
            red->changeCenter(-cen);
            red->track(rotAng, rotAxi);
            red->changeCenter(cen);
			red->changeCenter(1.0f * moves[RIGHT]);
            break;
		case GLFW_KEY_R:
            cen = rAstro->getCen();
            red->changeCenter(-cen);
            red->track(90, glm::vec3(0, 1, 0));
            red->changeCenter(cen);
			break;
        case GLFW_KEY_L:
            cen = rAstro->getCen();
            red->changeCenter(-cen);
            red->track(-90, glm::vec3(0, 1, 0));
            red->changeCenter(cen);
			break;

		default:
			break;
		}
    }
    

}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        Movement = ROTATE;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        lastX = xpos;
        lastY = ypos;
        
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        Movement = NONE;
        
    }
    
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    float xoffset, yoffset, sensitivity;
    
    switch (Movement) {
    case ROTATE:
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        xoffset = -(xpos-lastX);
        yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;

        sensitivity = 0.5f; // change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) {
            pitch = 89.0f;
            yoffset = 0;
        }
        if (pitch < -89.0f) {
            pitch = -89.0f;
            yoffset = 0;
        }

        
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(yoffset), glm::normalize(glm::cross(upVector, eyePos)));
        eyePos = glm::vec3(rot * glm::vec4(eyePos, 1.0));
        
        rot = glm::rotate(glm::mat4(1.0f), glm::radians(xoffset), upVector);
        eyePos = glm::vec3(rot * glm::vec4(eyePos, 1.0));

        view = glm::lookAt(eyePos, lookAtPoint, upVector);// update view vector
        
		break;

	case NONE:
		break;
            
    }

}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 60.0f)
        fov = 60.0f;

    Window::projection = glm::perspective(glm::radians(fov),
        (float)width / (float)height, 1.0f, 1000.0f);
}

bool Window::sphereCollision(Geometry* s1, Geometry* s2) {
    glm::vec3 cen1, cen2;
    float r1, r2;
    cen1 = s1->getCen();
    cen2 = s2->getCen();
    r1 = s1->getRadius();
    r2 = s2->getRadius();

    if (glm::length(cen2-cen1) < r1 + r2) {
        return true;
    } else {
        return false;
    }

 
}

bool Window::spherePlaneCollision(Geometry* s1, Window::Plane p) {
    glm::vec3 cen1 = s1->getCen();
    float r1 = s1->getRadius();
    float dist = abs(glm::dot((cen1 - p.point), p.norm));

    if (dist < r1) {
        return true;
    }
    else {
        return false;
    }

}