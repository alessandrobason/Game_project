#include "Player.h"
#include "Framework/RoomManager.h"

Player::Player() : GameObject(nullptr, nullptr) {}

Player::Player(InputHandler* input, RoomManager* rm, sf::RenderWindow* win) : GameObject(input, win){
    in = input;
    w = win;

    // load player data from json
    config = new JSONparser("player_data.json");

    int w = config->doc["collision"].obj["width"].i;
    int h = config->doc["collision"].obj["height"].i;
    collider = Collision(0, 0, w, h);

    roommanager = rm;

    if (!roommanager->textures["player"].loadFromFile(config->doc["spritesheet"].obj["image"].str)) {
        std::cout << "Failed to load " << config->doc["spritesheet"].str << "\n";
    }

    int rows = config->doc["spritesheet"].obj["rows"].i;
    int columns = config->doc["spritesheet"].obj["columns"].i;

    // animated sprite
    animSprite.setSpriteSheet(&roommanager->textures["player"], columns, rows);

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
}

void Player::setPosition(sf::Vector2f pos) {
    getSprite()->setPosition(pos);
    collider.rect = sf::FloatRect(pos.x, pos.y, collider.rect.width, collider.rect.height);
}

void Player::setDirection(int dir) {
    last_direction = (DIRECTIONS)dir;
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
    oldVel = vel;
    vel = sf::Vector2f(0, 0);

    vel.x = in->isKeyDown(in->KEY_RIGHT) - in->isKeyDown(in->KEY_LEFT);
    vel.y = in->isKeyDown(in->KEY_DOWN) - in->isKeyDown(in->KEY_UP);

    sf::Vector2f mousepos = (sf::Vector2f) in->getMouseRelative();
    sf::Vector2f center = (sf::Vector2f) w->getSize()/2.f;
    center.x = mousepos.x - center.x;
    center.y = mousepos.y - center.y;
    float angle = (std::atan2(center.y, center.x)*180/M_PI + 180) / 45;

    switch ((int)angle) {
    case 0:
    case 7:
        last_direction = LEFT;
        break;
    case 1:
    case 2:
        last_direction = UP;
        break;
    case 3:
    case 4:
        last_direction = RIGHT;
        break;
    case 5:
    case 6:
        last_direction = DOWN;
        break;
    }

    //std::cout << (int)angle << "\n";
   
    float l = std::sqrt(vel.x * vel.x + vel.y * vel.y);
    if (l != 0)	vel /= l;
    vel *= speed;

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
}

void Player::update(float dt) {
    animSprite.animate(dt);
    move(vel * dt);
    //std::cout << "spr pos:\t" << getSprite()->getPosition().x << " " << getSprite()->getPosition().y << "\n";
}

void Player::draw() {
    animSprite.draw(w);
}
