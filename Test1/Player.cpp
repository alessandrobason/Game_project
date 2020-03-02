#include "Player.h"
#include "Framework/RoomManager.h"

Player::Player() : GameObject(nullptr, nullptr){}

Player::Player(InputHandler* input, RoomManager* rm, sf::RenderWindow* win) : GameObject(input, win){
    in = input;
    w = win;

    // load player data from json
    config = new JSONparser("player_data.json");

    int x = config->doc["collision"].obj["x"].i;
    int y = config->doc["collision"].obj["y"].i;
    int w = config->doc["collision"].obj["width"].i;
    int h = config->doc["collision"].obj["height"].i;
    collider = Collision(x, y, w, h, Collision::PLAYER);

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
    animSprite.setCurrentAnimation("idle down-right");

    local_center = collider.collision_offset + sf::Vector2f(collider.rect.width / 2, collider.rect.height / 2);
    
    // load weapon

    if (!roommanager->textures["weapons"].loadFromFile(config->doc["weapons"].obj["spritesheet"].str)) {
        std::cout << "Failed to load weapons texture from " << config->doc["weapons"].obj["spritesheet"].str << "\n";
    }

    sf::Vector2i bow_texture_coordinates;
    bow_texture_coordinates.x = config->doc["weapons"].obj["bow"].obj["texture coordinates"].arr[0].i;
    bow_texture_coordinates.y = config->doc["weapons"].obj["bow"].obj["texture coordinates"].arr[1].i;
    bow = Weapon(&roommanager->textures["weapons"], bow_texture_coordinates, in);
}

void Player::move(sf::Vector2f offset) {
    GameObject::move(offset);
    bow.move(offset);
}

void Player::setPosition(sf::Vector2f pos) {
    getSprite()->setPosition(pos);
    collider.rect = sf::FloatRect(pos.x + collider.collision_offset.x, pos.y + collider.collision_offset.y, collider.rect.width, collider.rect.height);
    bow.setPosition(pos + local_center);
}

void Player::setDirection(int dir) {
    last_direction = (DIRECTIONS)dir;
    switch (last_direction) {
    case UP_RIGHT:
        animSprite.setCurrentAnimation("idle up-right");
        break;
    case UP_LEFT:
        animSprite.setCurrentAnimation("idle up-left");
        break;
    case DOWN_RIGHT:
        animSprite.setCurrentAnimation("idle down-right");
        break;
    case DOWN_LEFT:
        animSprite.setCurrentAnimation("idle down-left");
        break;
    }
}

void Player::handleInput(float dt) {
    oldVel = vel;
    vel = sf::Vector2f(0, 0);

    vel.x = in->isKeyDown(in->KEY_RIGHT) - in->isKeyDown(in->KEY_LEFT);
    vel.y = in->isKeyDown(in->KEY_DOWN) - in->isKeyDown(in->KEY_UP);

    sf::Vector2f mousepos = (sf::Vector2f) in->getMouseRelative();

    float scale = w->getDefaultView().getSize().x / w->getView().getSize().x;
    sf::Vector2f player_local_position = getSprite()->getPosition() + local_center - roommanager->getCurrentRoom()->getCameraTopLeft();
    sf::Vector2f center = player_local_position * scale;

    center.x = mousepos.x - center.x;
    center.y = mousepos.y - center.y;
    angle_to_mouse = std::atan2(center.y, center.x)*180/M_PI + 180;
    bow.setAngle(angle_to_mouse);

    switch ((int)(angle_to_mouse / 45)) {
    case 0:
    case 1:
        last_direction = UP_LEFT;
        draw_weapon_over = false;
        break;
    case 2:
    case 3:
        last_direction = UP_RIGHT;
        draw_weapon_over = false;
        break;
    case 4:
    case 5:
        last_direction = DOWN_RIGHT;
        draw_weapon_over = true;
        break;
    case 6:
    case 7:
        last_direction = DOWN_LEFT;
        draw_weapon_over = true;
        break;
    }
   
    float l = std::sqrt(vel.x * vel.x + vel.y * vel.y);
    if (l != 0)	vel /= l;
    vel *= speed;

    std::string anim = "";
    if (vel != sf::Vector2f(0, 0)) anim += "walk ";
    else anim += "idle ";

    switch (last_direction) {
    case UP_LEFT:
        anim += "up-left";
        break;
    case UP_RIGHT:
        anim += "up-right";
        break;
    case DOWN_RIGHT:
        anim += "down-right";
        break;
    case DOWN_LEFT:
        anim += "down-left";
        break;
    }

    animSprite.setCurrentAnimation(anim);
    bow.handleInput(dt);
}

void Player::update(float dt) {
    animSprite.animate(dt);
    move(vel * dt);
    bow.update(dt);
    //std::cout << "spr pos:\t" << getSprite()->getPosition().x << " " << getSprite()->getPosition().y << "\n";
}

void Player::draw() {
    if (!draw_weapon_over) w->draw(bow);
    animSprite.draw(w);
    if (draw_weapon_over) w->draw(bow);
    bow.drawArrows(w);
}

void Player::drawDebug(){
    GameObject::drawDebug();
    bow.drawDebug(w);
}