#pragma once

#include "cloth.hpp"

class RectClothSimulator {
private:
    struct MassParticle {
        glm::vec3 position;
        std::vector<unsigned int> connectedSpringStartIndices;
        std::vector<unsigned int> connectedSpringEndIndices;

        // TODO: define other particle properties here
        // MY CODE HERE
        glm::vec3 velocity;
        float mass;
        glm::vec3 force;
        // MY CODE END
    };

    struct Spring
    {
        unsigned int fromMassIndex;
        unsigned int toMassIndex;

        // TODO: define other spring properties here
        // MY CODE HERE
        float stiffness;
        float restLength;
        // MY CODE END

    };

    RectCloth* cloth;
    std::vector<MassParticle> particles;
    std::vector<Spring> springs;

    // Simulation parameters
    glm::vec3 gravity;
    float airResistanceCoefficient; // Per-particle

    // collision parameters
    float collision_radius = 1.0f;
    glm::vec3 center = glm::vec3(0.1f, -2.0f, -0.3f);

public:
    RectClothSimulator(
            RectCloth* cloth,
            float totalMass,
            float stiffnessReference,
            float airResistanceCoefficient,
            const glm::vec3& gravity);
    ~RectClothSimulator() = default;

    void step(float timeStep);
    bool is_wind;
    bool is_collision;

private:
    void createMassParticles(float totalMass);
    void createSprings(float stiffnessReference);
    void updateCloth();
};