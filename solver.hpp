#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "utils/math.hpp"

struct VerletObject {
    sf::Vector2f position;
    sf::Vector2f position_last;
    sf::Vector2f acceleration;
    float radius = 10.0f;
    sf::Color color = sf::Color::White;

    VerletObject()
        : position{0.0f, 0.0f}, position_last{0.0f, 0.0f}, acceleration{0.0f, 0.0f}{}
    
    VerletObject(sf::Vector2f position_, float radius_)
        : position{position_}, position_last{position_}, acceleration{0.0f, 0.0f}, radius{radius_} {}
    
    void Update(float dt) {
        const sf::Vector2f displacement = position - position_last;
        position_last = position;
        position += displacement + acceleration * (dt * dt);
        acceleration = {};
    }

    void Accelerate(sf::Vector2f a){
        acceleration += a;
    }

    void SetVelocity(sf::Vector2f v, float dt){
        position_last = position - (v * dt);
    }

    void AddVelocity(sf::Vector2f v, float dt){
        position_last -= v * dt;
    }

    [[nodiscard]]
    sf::Vector2f GetVelocity(float dt) const {
        return (position - position_last) / dt;
    }
};

class Solver{
public:
    Solver() = default;

    VerletObject & AddObject(const sf::Vector2f position, float radius){
        return objects_.emplace_back(position, radius);
    }

    void Update(){
        time_ += frame_dt_;
        const float step_dt = GetStepDt();
        for (uint32_t i = 0; i < sub_steps_; ++i) {
            ApplyGravity();
            CheckCollisions(step_dt);
            ApplyConstraint();
            UpdateObjects(step_dt);
        }
    }

    void SetSimulationUpdateRule(uint32_t rate) {
        frame_dt_ = 1.0f/ static_cast<float>(rate);
    }

    void SetConstraint(const sf::Vector2f& position, float radius) {
        constraint_center_ = position;
        constraint_radius_ = radius;
    }

    void SetSubStepsCount(uint32_t sub_steps) {
        sub_steps_ = sub_steps;
    }

    void SetObjectVelocity(VerletObject& object, const sf::Vector2f& velocity) {
        object.SetVelocity(velocity, GetStepDt());
    }

    [[nodiscard]] const std:: vector<VerletObject>& GetObjects() const{
        return objects_;
    }

    [[nodiscard]] sf:: Vector3f GetConstraint() const {
        return {constraint_center_.x, constraint_center_.y, constraint_radius_};
    }

    [[nodiscard]] uint64_t GetObjectsCount() const {
        return objects_.size();
    }

    [[nodiscoard]] float GetTime() const {
        return time_;
    }

    [[nodiscard]] float GetStepDt() const {
        return frame_dt_ / static_cast<float>(sub_steps_);
    }
    
private:
    uint32_t sub_steps_ = 1;
    sf::Vector2f gravity_ = {0.0f, 1000.0f};
    sf::Vector2f constraint_center_;
    float constraint_radius_ = 100.0f;
    std::vector<VerletObject> objects_;
    float time_ = 0.0f;
    float frame_dt_ = 0.0f;
    
    void ApplyGravity() {
        for (auto && object: objects_){
            object.Accelerate(gravity_);
        }
    }

    void CheckCollisions(float dt) {
        const float response_coef = 0.75f;
        const uint64_t object_count = objects_.size();

        for (uint64_t i = 0; i < object_count; ++i) {
            VerletObject& object_1 = objects_[i];

            for (uint64_t k = i + 1; k < object_count; ++k) {
                VerletObject& object_2 = objects_[k];
                const sf::Vector2f delta_position = object_1.position - object_2.position;
                const float dist2 = delta_position.x * delta_position.x + delta_position.y  * delta_position.y;
                const float min_dist = object_1.radius + object_2.radius;

                if (dist2 < min_dist * min_dist) {
                    const float distance  = std::sqrt(dist2);
                    const sf::Vector2f normal = delta_position / distance;
                    const float mass_ratio_1 = object_1.radius / (object_1.radius + object_2.radius);
                    const float mass_ratio_2 = object_2.radius / (object_1.radius + object_2.radius);
                    const float delta = 0.5f * response_coef * (distance - min_dist);

                    object_1.position -= normal * (mass_ratio_2 * delta);
                    object_2.position += normal * (mass_ratio_1 * delta);
                }
        }
    }
    }

    void ApplyConstraint() {
        for (auto& object : objects_) {
            const sf::Vector2f delta = constraint_center_ - object.position;
            const float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);

            if (dist > (constraint_radius_ - object.radius)) {
                const sf::Vector2f normal = delta / dist;
                object.position = constraint_center_ - normal * (constraint_radius_ - object.radius);
            }
        }
    }

    void UpdateObjects(float dt) {
        for (auto& object : objects_) {
            object.Update(dt);
        }
    }
};