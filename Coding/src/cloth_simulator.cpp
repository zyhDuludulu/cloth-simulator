#include <cmath>
#include <iostream>
#include "cloth_simulator.hpp"
#include "GLFW/glfw3.h"

glm::vec3 wind(glm::vec3 position, float time) {
    return glm::vec3(sin(position.x * position.y * time), cos(position.z * time), sin(cos(5 * position.x * position.y * position.z)));
}

RectClothSimulator::
RectClothSimulator(
        RectCloth *cloth,
        float totalMass,
        float stiffnessReference,
        float airResistanceCoefficient,
        const glm::vec3& gravity) : cloth(cloth), airResistanceCoefficient(airResistanceCoefficient), gravity(gravity) {
    // Initialize particles, then springs according to the given cloth
    createMassParticles(totalMass);
    createSprings(stiffnessReference);
}

void RectClothSimulator::
createMassParticles(float totalMass) {
    // Create mass particles based on given cloth.
    particles.resize(cloth->nw * cloth->nh);
    for (unsigned int ih = 0; ih < cloth->nh; ih++) {
        for (unsigned int iw = 0; iw < cloth->nw; iw++) {
            MassParticle particle;
            particle.position = cloth->getPosition(iw, ih);

            // TODO: Initialize other mass properties.
            //  Use 'cloth->...' to access cloth properties.
            // MY CODE HERE
            particle.velocity = glm::vec3(0.0f);
            particle.force = glm::vec3(0.0f);
            particle.mass = totalMass / cloth->nh / cloth->nw;
            // MY CODE END
            

            particles[cloth->idxFromCoord(iw, ih)] = particle;
        }
    }
}

void RectClothSimulator::
createSprings(float stiffnessReference) {
    // First clear all springs
    springs.clear();

    // TODO: Create springs connecting mass particles.
    //  You may find 'cloth->idxFromCoord(...)' useful.
    //  You can store springs into the member variable 'springs' which is a std::vector.
    //  You may want to modify mass particles too.
    // MY CODE HERE
    for (unsigned int ih = 0; ih < cloth->nh; ih++) {
        for (unsigned int iw = 0; iw < cloth->nw; iw++) {
            // right
            if (iw < cloth->nw - 1) {
                Spring spring;
                spring.fromMassIndex = cloth->idxFromCoord(iw, ih);
                spring.toMassIndex = cloth->idxFromCoord(iw + 1, ih);
                spring.stiffness = stiffnessReference;
                spring.restLength = cloth->dx;
                springs.push_back(spring);
                particles[spring.fromMassIndex].connectedSpringStartIndices.push_back(springs.size() - 1);
                particles[spring.toMassIndex].connectedSpringEndIndices.push_back(springs.size() - 1);
            }
            // down
            if (ih < cloth->nh - 1) {
                Spring spring;
                spring.fromMassIndex = cloth->idxFromCoord(iw, ih);
                spring.toMassIndex = cloth->idxFromCoord(iw, ih + 1);
                spring.stiffness = stiffnessReference;
                spring.restLength = cloth->dx;
                springs.push_back(spring);
                particles[spring.fromMassIndex].connectedSpringStartIndices.push_back(springs.size() - 1);
                particles[spring.toMassIndex].connectedSpringEndIndices.push_back(springs.size() - 1);
            }
            // right down
            if (iw < cloth->nw - 1 && ih < cloth->nh - 1) {
                Spring spring;
                spring.fromMassIndex = cloth->idxFromCoord(iw, ih);
                spring.toMassIndex = cloth->idxFromCoord(iw + 1, ih + 1);
                spring.stiffness = stiffnessReference;
                spring.restLength = cloth->dx * sqrt(2.0f);
                springs.push_back(spring);
                particles[spring.fromMassIndex].connectedSpringStartIndices.push_back(springs.size() - 1);
                particles[spring.toMassIndex].connectedSpringEndIndices.push_back(springs.size() - 1);
            }
            // left down
            if (iw > 0 && ih < cloth->nh - 1) {
                Spring spring;
                spring.fromMassIndex = cloth->idxFromCoord(iw, ih);
                spring.toMassIndex = cloth->idxFromCoord(iw - 1, ih + 1);
                spring.stiffness = stiffnessReference;
                spring.restLength = cloth->dx * sqrt(2.0f);
                springs.push_back(spring);
                particles[spring.fromMassIndex].connectedSpringStartIndices.push_back(springs.size() - 1);
                particles[spring.toMassIndex].connectedSpringEndIndices.push_back(springs.size() - 1);
            }
            // left
            if (iw > 0) {
                Spring spring;
                spring.fromMassIndex = cloth->idxFromCoord(iw, ih);
                spring.toMassIndex = cloth->idxFromCoord(iw - 1, ih);
                spring.stiffness = stiffnessReference;
                spring.restLength = cloth->dx;
                springs.push_back(spring);
                particles[spring.fromMassIndex].connectedSpringStartIndices.push_back(springs.size() - 1);
                particles[spring.toMassIndex].connectedSpringEndIndices.push_back(springs.size() - 1);
            }
            // up
            if (ih > 0) {
                Spring spring;
                spring.fromMassIndex = cloth->idxFromCoord(iw, ih);
                spring.toMassIndex = cloth->idxFromCoord(iw, ih - 1);
                spring.stiffness = stiffnessReference;
                spring.restLength = cloth->dx;
                springs.push_back(spring);
                particles[spring.fromMassIndex].connectedSpringStartIndices.push_back(springs.size() - 1);
                particles[spring.toMassIndex].connectedSpringEndIndices.push_back(springs.size() - 1);
            }
            // right up
            if (iw < cloth->nw - 1 && ih > 0) {
                Spring spring;
                spring.fromMassIndex = cloth->idxFromCoord(iw, ih);
                spring.toMassIndex = cloth->idxFromCoord(iw + 1, ih - 1);
                spring.stiffness = stiffnessReference;
                spring.restLength = cloth->dx * sqrt(2.0f);
                springs.push_back(spring);
                particles[spring.fromMassIndex].connectedSpringStartIndices.push_back(springs.size() - 1);
                particles[spring.toMassIndex].connectedSpringEndIndices.push_back(springs.size() - 1);
            }
            // left up
            if (iw > 0 && ih > 0) {
                Spring spring;
                spring.fromMassIndex = cloth->idxFromCoord(iw, ih);
                spring.toMassIndex = cloth->idxFromCoord(iw - 1, ih - 1);
                spring.stiffness = stiffnessReference;
                spring.restLength = cloth->dx * sqrt(2.0f);
                springs.push_back(spring);
                particles[spring.fromMassIndex].connectedSpringStartIndices.push_back(springs.size() - 1);
                particles[spring.toMassIndex].connectedSpringEndIndices.push_back(springs.size() - 1);
            }
        }
    }
}

void RectClothSimulator::
step(float timeStep) {
    // TODO: Simulate one step based on given time step.
    //  Step 1: Update particle positions
    //  Step 2: Update springs
    //  Step 3: Apply constraints
    //  Hint: See cloth_simulator.hpp to check for member variables you need.
    //  Hint: You may use 'cloth->getInitialPosition(...)' for constraints.
    // MY CODE HERE
    // Step 1
    for (unsigned int i = 0u; i < particles.size(); i++)
    {
        if (!is_collision && (i == 0u || i == cloth->nw - 1)) { continue; }
        particles[i].force += gravity * particles[i].mass;
        
        glm::vec3 airResistance = particles[i].velocity == glm::vec3(0.0f) ? 
            glm::vec3(0.0f) : -airResistanceCoefficient 
                * dot(particles[i].velocity, particles[i].velocity) 
                * normalize(particles[i].velocity);
        particles[i].force += airResistance;
        particles[i].velocity += particles[i].force / particles[i].mass * timeStep;
        particles[i].position += particles[i].velocity * timeStep;
        particles[i].force = glm::vec3(0.0f);
    }
    // Step 2
    for (unsigned int i = 0u; i < springs.size(); i++)
    {
        MassParticle& fromMass = particles[springs[i].fromMassIndex];
        MassParticle& toMass = particles[springs[i].toMassIndex];
        glm::vec3 springVector = toMass.position - fromMass.position;
        float springLength = glm::length(springVector);
        glm::vec3 springDirection = springVector / springLength;
        float springForce = springs[i].stiffness * (springLength - springs[i].restLength);
        fromMass.force += springDirection * springForce;
        toMass.force += -springDirection * springForce;
    }
    // Step 3
    if (is_wind) {
        for (unsigned int i = 0u; i < particles.size(); i++) {
            particles[i].force += 0.01f * wind(particles[i].position, glfwGetTime());   
        }
    } else if (is_collision) {
        for (unsigned int i = 0u; i < particles.size(); i++) {
            if (glm::length(particles[i].position - center) < collision_radius) {
                particles[i].position = glm::normalize(particles[i].position - center) * collision_radius + center;
            }
        }
    }
    // MY CODE END

    // Finally update cloth data
    updateCloth();
}

void RectClothSimulator::
updateCloth() {
    for (unsigned int i = 0u; i < cloth->nw * cloth->nh; i++)
    {
        cloth->setPosition(i, particles[i].position);
    }
}