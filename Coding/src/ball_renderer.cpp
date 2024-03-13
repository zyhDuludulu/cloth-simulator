#include "ball_renderer.hpp"

#define PI 3.1415926f

BallRenderer::
BallRenderer(
    Shader* shader,
    FirstPersonCamera* camera
) {
    this->shader = shader;
    this->camera = camera;

    this->initVertices();
    this->initIndices();

    this->glo.initData();
}

void BallRenderer::
draw() {

    // Update Data
    glBindBuffer(GL_ARRAY_BUFFER, glo.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * this->glo.vertices.size(), this->glo.vertices.data(), GL_STREAM_DRAW);

    GLint previous;
    glGetIntegerv(GL_POLYGON_MODE, &previous);

    this->shader->use();
    this->shader->setMat4("Projection", this->camera->getProjection());
    this->shader->setMat4("View", this->camera->getView());
    this->shader->setVec3("CameraPos", this->camera->getCameraPos());

    glBindVertexArray(glo.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, glo.VBO);

    this->shader->setBool("DrawLine", false);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FLAT); // We want flat mode
    glDrawElements(GL_TRIANGLES, (int)glo.indices.size(), GL_UNSIGNED_INT, NULL);

    // this->shader->setBool("DrawLine", true);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // We want line mode
    // glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(glo.indices.size()), GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, previous); // restore previous mode
}

void BallRenderer::
initVertices() {
    for (unsigned int i = 0; i < segments; ++i) {
        for (unsigned int j = 0; j < segments; ++j) {
            float theta = 2 * PI * i / segments;
            float phi = PI * j / segments;

            float x = radius * sin(phi) * cos(theta);
            float y = radius * cos(phi);
            float z = radius * sin(phi) * sin(theta);

            VertexData v;
            v.position = glm::vec3(x, y, z) + center;
            v.normal = glm::normalize(v.position - center);

            this->glo.vertices.push_back(v);
        }
    }
}


void BallRenderer::
initIndices() {
    for (unsigned int i = 0; i < segments; ++i) {
        for (unsigned int j = 0; j < segments; ++j) {
            unsigned int a = i * segments + j;
            unsigned int b = i * segments + (j + 1) % segments;
            unsigned int c = (i + 1) % segments * segments + j;
            unsigned int d = (i + 1) % segments * segments + (j + 1) % segments;

            this->glo.indices.push_back(a);
            this->glo.indices.push_back(b);
            this->glo.indices.push_back(c);

            this->glo.indices.push_back(b);
            this->glo.indices.push_back(c);
            this->glo.indices.push_back(d);
        }
    }
}
