#include "ParticleSystem.h"

// particle struct functions
void ParticleSystem::Particle::intialize(bool spawn) {

	// keep particles apart if handling a despawn
	float scale = 3.0f;

	// place particles closer together if handling a spawn
	if (spawn) {
		scale = 1.0f;
	}

	// intialize random position
	position.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/scale) - scale/2;
	position.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/scale) - scale/2;
	position.z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/scale) - scale/2;

	life = 0;

}

void ParticleSystem::Particle::update(bool spawn) {
	
	float scale = 1.001f;

	if (!spawn) {
		scale = 1/scale;
	}

	position *= scale;
	
	life++;
}

//particle system functions
ParticleSystem::ParticleSystem() : source(glm::vec3(0)), vis(false) {
	srand((unsigned)time(NULL)); // rng

	//default color
	color = glm::vec3(1, 1, 0);

	// intialize particles 
	for (int i = 0; i < MAX_PARTICLES; i++) {
		Particle p;
		p.intialize(true);

		particles.push_back(p);
		position_data.push_back(p.position);

	}

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * position_data.size(), position_data.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

ParticleSystem::~ParticleSystem()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void ParticleSystem::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader) {

	// dont draw particles when not visible
	if (!vis) {
		return;
	}

	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));

	// Bind the VAO
	glBindVertexArray(VAO);

	// Set point size
	glPointSize(3.0f);

	// Draw the points 
	glDrawArrays(GL_POINTS, 0, position_data.size());

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);


}

void ParticleSystem::update(bool spawn) {
	//check lifespan
	if (particles[0].life >= PARTICLE_LIFE) {
		// toggle particles off
		setVis(false);
		return;
	}


	//update particles
	for (int i = 0; i < MAX_PARTICLES; i++) {
		Particle p = particles[i];
		p.update(spawn);

		particles[i] = p;
		position_data[i] = p.position + source;
	}

	// reset vbo
	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * position_data.size(), position_data.data());
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void ParticleSystem::reset(bool spawn) {
	setVis(true);	// draw particles

	// reintialize particles
	for (int i = 0; i < MAX_PARTICLES; i++) {
		Particle p;
		p.intialize(spawn);

		particles[i] = p;
		position_data[i] = p.position + source;
	}

	// reset vbo
	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, position_data.size(), position_data.data());
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}

void ParticleSystem::setSource(glm::vec3 cen) {
	source = cen;
}

void ParticleSystem::setColor(glm::mat3 col) {
	color = col[0];
}

void ParticleSystem::setVis(bool state) {
	vis = state;
	
}