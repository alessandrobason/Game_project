#include "Player.h"

Player::Player() {}

Player::Player(InputHandler* input, Room* r, sf::Vector2f pos) {
    in = input;

    // load player data from json
    config = new JSONparser("player_data.json");

    int x = pos.x;
    int y = pos.y;
    int w = config->doc["collision"].obj["width"].i;
    int h = config->doc["collision"].obj["height"].i;
    collider = Collision(x, y, w, h);

    room = r;
    sf::Texture playerTemp;

    if (!playerTemp.loadFromFile(config->doc["spritesheet"].obj["image"].str)) {
        std::cout << "Failed to load " << config->doc["spritesheet"].str << "\n";
    }

    room->textures->insert(std::pair<std::string, sf::Texture>("player", playerTemp));

    int rows = config->doc["spritesheet"].obj["rows"].i;
    int columns = config->doc["spritesheet"].obj["columns"].i;

    // animated sprite
    animSprite.setSpriteSheet(&room->textures->at("player"), columns, rows);

    for (size_t i = 0; i < config->doc["animations"].arr.size(); i++) {
        std::vector<int> animFrames;
        float speed = (float) config->doc["animations"].arr[i].obj["speed"].d;
        bool looping = config->doc["animations"].arr[i].obj["looping"].b;
        for (size_t j = 0; j < config->doc["animations"].arr[i].obj["array"].arr.size(); j++) {
            animFrames.push_back(config->doc["animations"].arr[i].obj["array"].arr[j].i);
        }
        animSprite.addAnimation(config->doc["animations"].arr[i].obj["name"].str, animFrames, speed, looping);
    }
    animSprite.setCurrentAnimation("idle down");

    getSprite()->setPosition(pos);
}

void Player::setPosition(sf::Vector2f pos) {
    getSprite()->setPosition(pos);
    collider.rect = sf::FloatRect(pos.x, pos.y, collider.rect.width, collider.rect.height);
}

void Player::setDirection(DIRECTIONS dir) {
    last_direction = dir;
    switch (last_direction) {
    case UP:
        animSprite.setCurrentAnimation("idle up");
        break;
    case DOWN:
        animSprite.setCurrentAnimation("idle down");
        break;
    case LEFT:
        animSprite.setCurrentAnimation("idle left");
        break;
    case RIGHT:
        animSprite.setCurrentAnimation("idle right");
        break;
    }
}

void Player::handleInput(float dt) {
    //bool up = false, down = false, left = false, right = false;
    oldVel = vel;
    vel = sf::Vector2f(0, 0);

    if (in->isKeyDown(in->KEY_UP)) {
        last_direction = UP;
        //up = true;
        vel.y -= 1;
    }
    if (in->isKeyDown(in->KEY_DOWN)) {
        last_direction = DOWN;
        //down = true;
        vel.y += 1;
    }
    if (in->isKeyDown(in->KEY_LEFT)) {
        last_direction = LEFT;
        //left = true;
        vel.x -= 1;
    }
    if (in->isKeyDown(in->KEY_RIGHT)) {
        last_direction = RIGHT;
        //right = true;
        vel.x += 1;
    }

    float l = std::sqrt(vel.x * vel.x + vel.y * vel.y);
    if (l != 0)	vel /= l;
    vel *= speed;
    std::cout << "AIUTOOO\n";
    std::string anim = "";
    if (vel != sf::Vector2f(0, 0)) anim += "walk ";
    else anim += "idle ";

    switch (last_direction) {
    case UP:
        anim += "up";
        break;
    case DOWN:
        anim += "down";
        break;
    case LEFT:
        anim += "left";
        break;
    case RIGHT:
        anim += "right";
        break;
    }

    animSprite.setCurrentAnimation(anim);

    /*
    if (vel != sf::Vector2f(0, 0)) {
        if (right) {
            animSprite.setCurrentAnimation("WALK_R");
        }
        else if (left) {
            animSprite.setCurrentAnimation("WALK_L");
        }
        else if (down) {
            animSprite.setCurrentAnimation("WALK_D");
        }
        else if (up) {
            animSprite.setCurrentAnimation("WALK_U");
        }
    }
    else {
        std::string cur = animSprite.getCurrentAnimation();
        if (cur == "WALK_U") {
            animSprite.setCurrentAnimation("IDLE_U");
        }
        else if (cur == "WALK_D") {
            animSprite.setCurrentAnimation("IDLE_D");
        }
        else if (cur == "WALK_L") {
            animSprite.setCurrentAnimation("IDLE_L");
        }
        else if (cur == "WALK_R") {
            animSprite.setCurrentAnimation("IDLE_R");
        }
    }
    */
}

void Player::update(float dt) {
    animSprite.animate(dt);
    move(vel * dt);
    //std::cout << getSprite()->getPosition().x << " " << getSprite()->getPosition().y << "\n";
}

void Player::draw(sf::RenderWindow* w) {
    animSprite.draw(w);
    //w->draw(*getSprite());
}
