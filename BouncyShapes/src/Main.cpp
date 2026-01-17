#include<iostream>
#include<fstream>
#include<memory>

#include "imgui.h"
#include "imgui-SFML.h"

#include<SFML/Graphics.hpp>

//function declaration
void pollEvent(sf::RenderWindow& window);
void loadFromFile(const char* filename, std::vector<GameShape*>& shapeVector);

//Structs and Classes
struct GameShape{

    sf::Shape* shapeType;
    bool draw { true };
    std::string name{};
    float speedX{};
    float speedY{};
    float colorF[3] {};
    uint8_t colorI[3] {};
    float scaler{ 1 };

    GameShape(std::string name, float posX, float posY, float speedX, float speedY, float r, float g, float b, float width, float height):
    name(name),
    speedX(speedX),
    speedY(speedY){
        colorI[0] = r;
        colorI[1] = g;
        colorI[2] = b;
        colorItoF();
        shapeType = new sf::RectangleShape({width, height});
        shapeType->setOrigin(shapeType->getGeometricCenter());
        shapeType->setPosition({ posX, posY });
        shapeType->setFillColor({colorI[0], colorI[1], colorI[2]});
        std::cout << "Created Rect at: " << posX << ' ' << posY << '\n';
    }
    GameShape(std::string name, float posX, float posY, float speedX, float speedY, float r, float g, float b, float radius):
    name(name),
    speedX(speedX),
    speedY(speedY){
        colorI[0] = r;
        colorI[1] = g;
        colorI[2] = b;
        colorItoF();
        shapeType = new sf::CircleShape(radius);
        shapeType->setOrigin(shapeType->getGeometricCenter());
        shapeType->setPosition({ posX, posY });
        shapeType->setFillColor({ colorI[0], colorI[1], colorI[2] });
        std::cout << "Created Circle at: " << posX << ' ' << posY << '\n';
    }
    ~GameShape() {
        std::cout << "Called Shape destructor\n";
        delete shapeType;
        shapeType = nullptr;
    };

    void colorItoF() {
        colorF[0] = uint8_t(colorI[0] * 255);
        colorF[1] = uint8_t(colorI[1] * 255);
        colorF[2] = uint8_t(colorI[2] * 255);
    }
    void colorFtoI() {
        colorI[0] = float(colorF[0] / 255);
        colorI[1] = float(colorF[1] / 255);
        colorI[2] = float(colorF[2] / 255);
    }
    void printContent() {
        std::cout << name << ": " << shapeType << " Color float: " << colorF[0] << colorF[1] << colorF[2] << " Color uInt: " << colorI[0] << colorI[1] << colorI[2] << '\n';
    }

};

//Main
int main() {
    
    std::vector<GameShape*> shapesInGame;
    unsigned int width = 1280;
    unsigned int height = 760;

    sf::RenderWindow window(sf::VideoMode({ width, height }), "BouncyShapes");
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window)) {
        std::cout << "Could not initialize window \n";
        std::exit(-1);
    }

    sf::Clock deltaClock;

    loadFromFile("config/config.txt", shapesInGame);

    //scale the imgui ui and text size by 2
    ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

    while (window.isOpen()) {
        //Event handling
        pollEvent(window);

        //Physics Update


        //Update ImGui
        ImGui::SFML::Update(window, deltaClock.restart());

        //Draw UI
        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        //Clear Window
        window.clear();
        //Draw window
        for (auto s : shapesInGame) {
            window.draw(*s->shapeType);
        }
        
        ImGui::SFML::Render(window);
        //Display Window
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

//Function Definitions
void pollEvent(sf::RenderWindow& window) {
    while (const auto event = window.pollEvent()) {
        ImGui::SFML::ProcessEvent(window, *event);

        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            std::cout << "key pressed with code: " << int(keyPressed->scancode) << '\n';
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                window.close();
        }
    }
}

void loadFromFile(const char* filename, std::vector<GameShape*>& shapeVector) {
    std::ifstream fin(filename);
    std::string temp;

    while (fin >> temp){
        if (temp == "Window") {
            int width;
            int height;
            fin >> width >> height;
            std::cout << width << ' ' << height << '\n';
        }
        else if (temp == "Font") {
            std::cout << "A Font \n";
        }
        else if (temp == "Circle") {
            std::string name;
            float posX;
            float posY;
            float speedX;
            float speedY;
            float r;
            float g;
            float b;
            float radius;
            fin >> name >> posX >> posY >> speedX >> speedY >> r >> g >> b >> radius;
            shapeVector.push_back(new GameShape(name, posX, posY, speedX, speedY, r, g, b, radius));
        }
        else if (temp == "Rectange") {
            std::string name;
            float posX;
            float posY;
            float speedX;
            float speedY;
            float r;
            float g;
            float b;
            float width;
            float height;
            fin >> name >> posX >> posY >> speedX >> speedY >> r >> g >> b >> width >> height;
            shapeVector.push_back(new GameShape(name, posX, posY, speedX, speedY, r, g, b, width, height));
        }
    }
}