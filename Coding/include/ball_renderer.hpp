#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "camera.hpp"
#include "shader.hpp"

class BallRenderer {
private:
    struct VertexData {
        glm::vec3 position;
        glm::vec3 normal;
    };

    struct GLObject {
        GLuint VAO;
        GLuint VBO; std::vector<VertexData> vertices;
        GLuint EBO; std::vector<GLuint> indices;

        GLObject() {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
        };
        ~GLObject() {
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            glDeleteVertexArrays(1, &VAO);
        };
        void initData() {
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

            glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertices.size(), vertices.data(), GL_STREAM_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                sizeof(VertexData),
                (void*)offsetof(VertexData, position)
            );

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                sizeof(VertexData),
                (void*)offsetof(VertexData, normal)
            );

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        };
    };

    Shader* shader;
    FirstPersonCamera* camera;
    glm::vec3 center = glm::vec3(0.1f, -2.0f, -0.3f);
    float radius = 0.98f;
    unsigned int segments = 20;

    GLObject glo;

public:
    BallRenderer(
        Shader* shader,
        FirstPersonCamera* camera
    );
    ~BallRenderer() = default;

    void draw();

private:
    void initVertices();
    void initIndices();

    static glm::vec3 calcNormal(
        const glm::vec3& v1,
        const glm::vec3& v2,
        const glm::vec3& v3
    ) { return glm::normalize(glm::cross(v2 - v1, v3 - v1)); };
};