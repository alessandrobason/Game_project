#include "Player.h"
#include "Framework/RoomManager.h"

Player::Player() : GameObject(nullptr, nullptr, nullptr){}

Player::Player(InputHandler* input, RoomManager* rm, sf::RenderWindow* win) : GameObject(input, rm, win){
    collisionlayer = Collision::LAYER::PLAYER;

    // load player data from json
    config = new JSONparser("player_data.json");

    setSpeed(config->doc["speed"].i);

    int x = config->doc["collision"].obj["x"].i;
    int y = config->doc["collision"].obj["y"].i;
    int w = config->doc["collision"].obj["width"].i;
    int h = config->doc["collision"].obj["height"].i;
    collider = Collision(x, y, w, h, collisionlayer);

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
    
    // LOAD WEAPONs
    if (!roommanager->textures["weapons"].loadFromFile(config->doc["weapons"].obj["spritesheet"].str)) {
        std::cout << "Failed to load weapons texture from " << config->doc["weapons"].obj["spritesheet"].str << "\n";
    }

    sf::Vector2i bow_texture_coordinates;
    bow_texture_coordinates.x = config->doc["weapons"].obj["bow"].obj["texture coordinates"].arr[0].i;
    bow_texture_coordinates.y = config->doc["weapons"].obj["bow"].obj["texture coordinates"].arr[1].i;
    bow = Weapon(&roommanager->textures["weapons"], bow_texture_coordinates, in, collisionlayer, roommanager);

    sf::FloatRect arrowhitbox;
    arrowhitbox.top = config->doc["weapons"].obj["arrow"].obj["hitbox"].arr[0].i;
    arrowhitbox.left = config->doc["weapons"].obj["arrow"].obj["hitbox"].arr[1].i;
    arrowhitbox.width = config->doc["weapons"].obj["arrow"].obj["hitbox"].arr[2].i;
    arrowhitbox.height = config->doc["weapons"].obj["arrow"].obj["hitbox"].arr[3].i;
    bow.setHitBox(arrowhitbox);
    bow.setDamage(config->doc["weapons"].obj["bow"].obj["base_damage"].d);

    //  CHECKBOX
    checkboxsize = sf::Vector2f(200, 200);
    checkbox = Collision(-checkboxsize.x / 2.f, -checkboxsize.y / 2.f, checkboxsize.x / 2.f, checkboxsize.y / 2.f, Collision::LAYER::PLAYER);
    checkbox.setDebugColor(sf::Color::Yellow);
}

void Player::move(sf::Vector2f offset) {
    GameObject::move(offset);
    bow.move(offset);
    checkbox.moveCollision(offset);
}

void Player::setPosition(sf::Vector2f pos) {
    getSprite()->setPosition(pos);
    collider.rect = sf::FloatRect(pos.x + collider.collision_offset.x, pos.y + collider.collision_offset.y, collider.rect.width, collider.rect.height);
    bow.setPosition(pos + local_center);
    checkbox.setCenter(pos + local_center);
}

void Player::hit(float damage) {
    std::cout << "Player hit\n";
}

void Player::handleInput(float dt) {
    oldVel = vel;
    vel = sf::Vector2f(0, 0);

    vel.x = in->isKeyDown(in->KEY_RIGHT) - in->isKeyDown(in->KEY_LEFT);
    vel.y = in->isKeyDown(in->KEY_DOWN) - in->isKeyDown(in->KEY_UP);

    sf::Vector2i mousepos = in->getMouseRelative();
    sf::Vector2i center = w->mapCoordsToPixel(getSprite()->getPosition() + local_center);

    center.x = mousepos.x - center.x;
    center.y = mousepos.y - center.y;
    angle_to_mouse = UsefulFunc::atan2(center.y, center.x)*180/M_PI + 180;
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

    vel = UsefulFunc::normalize(vel) * speed;

    std::string anim = "idle ";
    if (vel != sf::Vector2f(0, 0)) anim = "walk ";

    anim += directionnames[last_direction];

    if (in->isKeyPressed(sf::Keyboard::Escape)) {
        roommanager->moveMenu("options");
    }

    animSprite.setCurrentAnimation(anim);
    bow.handleInput(dt);
}

void Player::update(float dt) {
    animSprite.animate(dt);
    bow.update(dt);
    if (canmove) {
        move(vel * dt);
    }
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
    checkbox.drawDebug(w);
}