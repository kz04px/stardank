#include <cmath>
#include <iostream>
#include <space/components/acceleration.hpp>
#include <space/components/ai_random.hpp>
#include <space/components/beam.hpp>
#include <space/components/body.hpp>
#include <space/components/commands.hpp>
#include <space/components/engine.hpp>
#include <space/components/fade.hpp>
#include <space/components/health.hpp>
#include <space/components/laser.hpp>
#include <space/components/render.hpp>
#include <space/components/targeter.hpp>
#include <space/components/velocity.hpp>
#include "../inputs.hpp"
#include "game.hpp"

using namespace space::components;

void commands(entt::registry &registry, const entt::entity us) {
    if (!registry.valid(us) || !registry.all_of<Commands>(us)) {
        return;
    }

    auto &commands = registry.get<Commands>(us);

    commands.forwards = Inputs::is_pressed(87);
    commands.backwards = Inputs::is_pressed(83);
    commands.left = Inputs::is_pressed(65);
    commands.right = Inputs::is_pressed(68);
    commands.shoot = Inputs::is_pressed(70);

    // Let's not have one input override the other
    if (commands.forwards && commands.backwards) {
        commands.forwards = false;
        commands.backwards = false;
    }
    if (commands.left && commands.right) {
        commands.left = false;
        commands.right = false;
    }
}

void engines(entt::registry &registry) {
    auto view = registry.view<Acceleration, Engine, const Body, const Commands>();

    for (auto entity : view) {
        auto &acc = view.get<Acceleration>(entity);
        auto &engine = view.get<Engine>(entity);
        const auto &body = view.get<const Body>(entity);
        const auto &commands = registry.get<const Commands>(entity);

        engine.forwards = commands.forwards;
        engine.backwards = commands.backwards;
        engine.cw = commands.right;
        engine.ccw = commands.left;

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
    auto view = registry.view<Velocity, Engine, const Body, const Commands>();

    for (auto entity : view) {
        auto &vel = view.get<Velocity>(entity);
        auto &engine = view.get<Engine>(entity);
        const auto &body = view.get<const Body>(entity);
        const auto &commands = registry.get<const Commands>(entity);

        engine.forwards = commands.forwards;
        engine.backwards = commands.backwards;
        engine.cw = commands.right;
        engine.ccw = commands.left;

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
    auto view = registry.view<Body, const Velocity>();  // , Acceleration

    for (auto entity : view) {
        auto &body = view.get<Body>(entity);
        const auto &vel = view.get<const Velocity>(entity);
        // const auto &acc = view.get<Acceleration>(entity);

        // s = ut + 1/2at^2
        body.x += vel.dx * dt;  // + 0.5 * acc.x * dt * dt;
        body.y += vel.dy * dt;  // + 0.5 * acc.y * dt * dt;

        // Angular
        body.r += vel.angular * dt;  // + 0.5 * acc.angular * dt * dt;

        // Normalise rotation
        if (body.r >= 2.0f * M_PI) {
            body.r -= 2.0f * M_PI;
        } else if (body.r < 0.0f) {
            body.r += 2.0f * M_PI;
        }
    }
}

void laser(entt::registry &registry, const float dt) {
    auto view = registry.view<const Body, const Targeter, const Commands, Laser>();

    for (auto entity : view) {
        const auto &targeter = registry.get<const Targeter>(entity);

        // Can't shoot ourselves
        if (targeter.target == entity) {
            continue;
        }

        // Target must be valid and have a body
        if (!registry.valid(targeter.target) || !registry.all_of<Body>(targeter.target)) {
            continue;
        }

        const auto &commands = view.get<const Commands>(entity);
        const auto &target_body = registry.get<const Body>(targeter.target);
        const auto &body = view.get<const Body>(entity);
        auto &laser = view.get<Laser>(entity);

        if (commands.shoot && laser.cooldown <= 0.0f) {
            const auto dx = body.x - target_body.x;
            const auto dy = body.y - target_body.y;
            const auto dist = sqrt(dx * dx + dy * dy);

            if (dist <= laser.range) {
                laser.cooldown = 1.0f;

                // Create beam entity
                auto beam = registry.create();
                registry.emplace<Beam>(beam, entity, targeter.target, Beam::Type::Damage, 3.0f, 1.0f, laser.range);
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

void ai_random(entt::registry &registry, const float dt) {
    auto view = registry.view<Commands, AIRandom, const Body>();

    for (auto entity : view) {
        auto &commands = registry.get<Commands>(entity);
        auto &ai = registry.get<AIRandom>(entity);
        const auto &body = registry.get<const Body>(entity);

        ai.duration += dt;

        // Reset commands
        commands.forwards = false;
        commands.backwards = false;
        commands.left = false;
        commands.right = false;
        commands.shoot = false;

        switch (ai.stage) {
            case AIRandom::Stage::Wait:
                if (ai.duration >= 1.0f) {
                    ai.stage = AIRandom::Stage::Turn;
                    ai.duration = 0.0f;
                    ai.memory[0] = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;
                }
                break;
            case AIRandom::Stage::Turn:
                if (ai.duration >= 3.0f) {
                    ai.stage = AIRandom::Stage::Travel;
                    ai.duration = 0.0f;
                    break;
                }

                if (body.r > ai.memory[0]) {
                    commands.left = true;
                    commands.right = false;
                } else if (body.r < ai.memory[0]) {
                    commands.left = false;
                    commands.right = true;
                } else {
                    commands.left = false;
                    commands.right = false;
                }
                break;
            case AIRandom::Stage::Travel:
                if (ai.duration >= 1.0f) {
                    ai.stage = AIRandom::Stage::Target;
                    ai.duration = 0.0f;
                    break;
                }

                commands.forwards = true;
                commands.left = false;
                commands.right = false;
                break;
            case AIRandom::Stage::Target:
                ai.stage = AIRandom::Stage::Wait;
                break;
            case AIRandom::Stage::Engage:
                commands.shoot = true;
                break;
            default:
                break;
        }

        // Let's not have one command override the other
        if (commands.forwards && commands.backwards) {
            commands.forwards = false;
            commands.backwards = false;
        }
        if (commands.left && commands.right) {
            commands.left = false;
            commands.right = false;
        }
    }
}

void fader(entt::registry &registry, const float dt) {
    auto view = registry.view<Fade, Render>();

    view.each([&registry, dt](auto &fade, auto &render) {
        fade.current += dt;
        fade.current = std::min(fade.total, fade.current);
        render.a = 1.0f - fade.current / fade.total;
    });
}

void Game::update(const float dt) {
    if (!m_map_view) {
        commands(m_registry, m_us);
        ai_random(m_registry, dt);
        magic_engines(m_registry);
        acceleration(m_registry, dt);
        position(m_registry, dt);
        laser(m_registry, dt);
        beam(m_registry, dt);
        health(m_registry);
        fader(m_registry, dt);

        const auto pos = m_registry.get<Body>(m_us);
        m_camera.position = {pos.x, pos.y};
    }
}
