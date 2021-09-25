#include <cmath>
#include <iostream>
#include <space/components/acceleration.hpp>
#include <space/components/beam.hpp>
#include <space/components/body.hpp>
#include <space/components/engine.hpp>
#include <space/components/health.hpp>
#include <space/components/laser.hpp>
#include <space/components/velocity.hpp>
#include "../inputs.hpp"
#include "game.hpp"

using namespace space::components;

void inputs(entt::registry &registry) {
    auto view = registry.view<Engine>();

    view.each([](auto &engine) {
        engine.forwards = Inputs::is_pressed(87);
        engine.backwards = Inputs::is_pressed(83);
        engine.cw = Inputs::is_pressed(68);
        engine.ccw = Inputs::is_pressed(65);

        // Let's not have one direction override the other
        if (engine.forwards && engine.backwards) {
            engine.forwards = false;
            engine.backwards = false;
        }
        if (engine.cw && engine.ccw) {
            engine.cw = false;
            engine.ccw = false;
        }
    });
}

void engines(entt::registry &registry) {
    auto view = registry.view<Acceleration, Body, Engine>();

    for (auto entity : view) {
        auto &acc = view.get<Acceleration>(entity);
        const auto &body = view.get<Body>(entity);
        const auto &engine = view.get<Engine>(entity);

        // Forwards/Backwards
        if (engine.forwards) {
            acc.x = engine.max_thrust * sin(body.r);
            acc.y = engine.max_thrust * cos(body.r);
        } else if (engine.backwards) {
            acc.x = -engine.max_thrust * sin(body.r);
            acc.y = -engine.max_thrust * cos(body.r);
        } else {
            acc.x = 0.0f;
            acc.y = 0.0f;
        }

        // CW/CCW
        if (engine.cw) {
            acc.angular = engine.max_rotation;
        } else if (engine.ccw) {
            acc.angular = -engine.max_rotation;
        } else {
            acc.angular = 0.0f;
        }
    }
}

void magic_engines(entt::registry &registry) {
    auto view = registry.view<Velocity, Body, Engine>();

    for (auto entity : view) {
        auto &vel = view.get<Velocity>(entity);
        const auto &body = view.get<Body>(entity);
        const auto &engine = view.get<Engine>(entity);

        // Forwards/Backwards
        if (engine.forwards) {
            vel.dx = engine.max_thrust * sin(body.r);
            vel.dy = engine.max_thrust * cos(body.r);

        } else if (engine.backwards) {
            vel.dx = -engine.max_thrust * sin(body.r);
            vel.dy = -engine.max_thrust * cos(body.r);
        } else {
            vel.dx = 0.0f;
            vel.dy = 0.0f;
        }

        // CW/CCW
        if (engine.cw) {
            vel.angular = engine.max_rotation;
        } else if (engine.ccw) {
            vel.angular = -engine.max_rotation;
        } else {
            vel.angular = 0.0f;
        }
    }
}

void acceleration(entt::registry &registry, const float dt) {
    auto view = registry.view<Velocity, const Acceleration>();

    view.each([dt](auto &vel, const auto &acc) {
        // v = u + at
        vel.dx += acc.x * dt;
        vel.dy += acc.y * dt;

        // ω = ω + at
        vel.angular += acc.angular * dt;
    });
}

void position(entt::registry &registry, const float dt) {
    auto view = registry.view<Body, Velocity>();  // , Acceleration

    for (auto entity : view) {
        auto &body = view.get<Body>(entity);
        const auto &vel = view.get<Velocity>(entity);
        // const auto &acc = view.get<Acceleration>(entity);

        // s = ut + 1/2at^2
        body.x += vel.dx * dt;  // + 0.5 * acc.x * dt * dt;
        body.y += vel.dy * dt;  // + 0.5 * acc.y * dt * dt;

        // Angular
        body.r += vel.angular * dt;  // + 0.5 * acc.angular * dt * dt;
    }
}

void laser(entt::registry &registry, const float dt, const entt::entity target) {
    // Gotta be a valid target
    if (!registry.valid(target)) {
        return;
    }

    /*
    // Target has to have a position
    if (!registry.has<Body>(target)) {
        return;
    }
    */

    const auto target_body = registry.get<Body>(target);
    auto view = registry.view<const Body, Laser>();

    for (auto entity : view) {
        // Can't shoot ourselves
        if (entity == target) {
            return;
        }

        const auto &body = view.get<const Body>(entity);
        auto &laser = view.get<Laser>(entity);

        if (Inputs::is_pressed(70) && laser.cooldown <= 0.0f) {
            const auto dx = body.x - target_body.x;
            const auto dy = body.y - target_body.y;
            const auto dist = sqrt(dx * dx + dy * dy);

            if (dist <= laser.range) {
                laser.cooldown = 1.0f;

                // Create beam entity
                auto beam = registry.create();
                registry.emplace<Beam>(beam, entity, target, Beam::Type::Damage, 3.0f, 1.0f, laser.range);
            }
        }

        // Reduce cooldown with time
        laser.cooldown -= dt;
    }
}

void beam(entt::registry &registry, const float dt) {
    auto view = registry.view<Beam>();

    view.each([&registry, dt](const entt::entity entity, auto &beam) {
        // Target must be valid
        if (!registry.valid(beam.target)) {
            registry.destroy(entity);
            return;
        }

        // Duration
        beam.duration -= dt;
        if (beam.duration <= 0.0f) {
            registry.destroy(entity);
            return;
        }

        // Range
        const auto from = registry.get<Body>(beam.parent);
        const auto to = registry.get<Body>(beam.target);
        const auto dx = to.x - from.x;
        const auto dy = to.y - from.y;
        const auto dist = sqrt(dx * dx + dy * dy);
        if (dist > beam.max_dist) {
            registry.destroy(entity);
            return;
        }

        // Effects
        switch (beam.type) {
            case Beam::Type::Damage: {
                if (registry.all_of<Health>(beam.target)) {
                    auto &health = registry.get<Health>(beam.target);
                    health.remaining -= beam.power * dt;
                }
                break;
            }
            default:
                break;
        }
    });
}

void health(entt::registry &registry) {
    auto view = registry.view<const Health>();

    view.each([&registry](const entt::entity entity, const auto &health) {
        if (health.remaining <= 0.0f) {
            registry.destroy(entity);
        }
    });
}

void Game::update(const float dt) {
    if (!m_map_view) {
        inputs(m_registry);
        magic_engines(m_registry);
        acceleration(m_registry, dt);
        position(m_registry, dt);
        laser(m_registry, dt, m_entity_selected);
        beam(m_registry, dt);
        health(m_registry);

        const auto pos = m_registry.get<Body>(m_us);
        m_camera.position = {pos.x, pos.y};
    }
}
