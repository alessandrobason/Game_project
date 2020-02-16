#include "Player.h"

Player::Player() {}

Player::Player(InputHandler* input, Room* r, std::string txt_path, sf::Vector2f pos) {
    in = input; 

    collider = Collision(0, 0, 16, 16);
    collider.setDebugPosition(sf::Vector2f(pos.x + 1, pos.y + 1));
    collider.setDebugSize(sf::Vector2f(14, 14));
    
    // load player data from json
    config = new JSONparser("player_data.json");
    
    room = r;
    room->textures["player"] = sf::Texture();

    if (!room->textures["player"].loadFromFile(txt_path)) {
        std::cout << "Failed to load " << txt_path << "\n";
    }

    txt = &room->textures["player"];

    // animated sprite
    animSprite.setSpriteSheet(txt, 8, 2);

    for (size_t i = 0; i < config->doc["animations"].arr.size(); i++) {
        std::vector<int> animFrames;
        std::cout << config->doc["animations"].arr[i].obj["name"].str << "\n";
        float speed = (float) config->doc["animations"].arr[i].obj["speed"].d;
        bool looping = config->doc["animations"].arr[i].obj["looping"].b;
        std::cout << "Speed: " << speed << "\n";
        std::cout << "Looping: " << looping << "\n";
        std::cout << "Animation: ";
        for (size_t j = 0; j < config->doc["animations"].arr[i].obj["array"].arr.size(); j++) {
            animFrames.push_back(config->doc["animations"].arr[i].obj["array"].arr[j].i);
            std::cout << config->doc["animations"].arr[i].obj["array"].arr[j].i << " ";
        }
        std::cout << "\n";
        animSprite.addAnimation(animFrames, speed, looping);
    }
    animSprite.setCurrentAnimation(IDLE_D);

    getSprite()->setPosition(pos);
}

void Player::handleInput(float dt) {
    bool up = false, down = false, left = false, right = false;
    oldVel = vel;
    vel = sf::Vector2f(0, 0);

    if (in->isKeyDown(in->KEY_UP)) {
        up = true;
        vel.y -= 1;
    }
    if (in->isKeyDown(in->KEY_DOWN)) {
        down = true;
        vel.y += 1;
    }
    if (in->isKeyDown(in->KEY_LEFT)) {
        left = true;
        vel.x -= 1;
    }
    if (in->isKeyDown(in->KEY_RIGHT)) {
        right = true;
        vel.x += 1;
    }

    float l = std::sqrt(vel.x * vel.x + vel.y * vel.y);
    if (l != 0)	vel /= l;
    vel *= speed;

    if (vel != sf::Vector2f(0, 0)) {
        if (right) {
            animSprite.setCurrentAnimation(WALK_R);
        }
        else if (left) {
            animSprite.setCurrentAnimation(WALK_L);
        }
        else if (down) {
            animSprite.setCurrentAnimation(WALK_D);
        }
        else if (up) {
            animSprite.setCurrentAnimation(WALK_U);
        }
    }
    else {
        switch (animSprite.getCurrentAnimation()) {
        case WALK_U:
            animSprite.setCurrentAnimation(IDLE_U);
            break;
        case WALK_D:
            animSprite.setCurrentAnimation(IDLE_D);
            break;
        case WALK_L:
            animSprite.setCurrentAnimation(IDLE_L);
            break;
        case WALK_R:
            animSprite.setCurrentAnimation(IDLE_R);
            break;
        }
    }
}

void Player::update(float dt) {
    animSprite.animate(dt);
    move(vel * dt);
}

void Player::draw(sf::RenderWindow* w) {
    animSprite.draw(w);
    w->draw(*getSprite());
}
