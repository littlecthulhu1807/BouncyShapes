#include<iostream>
#include<fstream>
#include<memory>

#include "imgui.h"
#include "imgui-SFML.h"

#include<SFML/Graphics.hpp>

//Structs and Classes
struct GameShape{

    sf::Shape* shapeType;
    bool draw { true };
    std::string name{};
    float speedX{};
    float speedY{};
    float colorF[3] {};
    float currentPosX{};
    float currentPosY{};
    uint8_t colorI[3] {};
    float scaler{ 1 };
    sf::Text* shapeText;

    GameShape(std::string name, float posX, float posY, float speedX, float speedY, float r, float g, float b, float width, float height):
    name(name),
    speedX(speedX),
    speedY(speedY),
    currentPosX(posX),
    currentPosY(posY){
        colorI[0] = uint8_t(r);
        colorI[1] = uint8_t(g);
        colorI[2] = uint8_t(b);
        colorItoF();
        shapeType = new sf::RectangleShape({width, height});
        shapeType->setOrigin(shapeType->getGeometricCenter());
        shapeType->setPosition({ currentPosX, currentPosY });
        shapeType->setFillColor({colorI[0], colorI[1], colorI[2]});
        std::cout << name << " || " << int(colorI[0]) << " || " << int(colorI[1]) << " || " << int(colorI[2]) << '\n';
        std::cout << name << " || " << colorF[0] << " || " << colorF[1] << " || " << colorF[2] << '\n';
    }
    GameShape(std::string name, float posX, float posY, float speedX, float speedY, float r, float g, float b, float radius):
    name(name),
    speedX(speedX),
    speedY(speedY),
    currentPosX(posX),
    currentPosY(posY) {
        colorI[0] = uint8_t(r);
        colorI[1] = uint8_t(g);
        colorI[2] = uint8_t(b);
        colorItoF();
        shapeType = new sf::CircleShape(radius);
        shapeType->setOrigin(shapeType->getGeometricCenter());
        shapeType->setPosition({ currentPosX, currentPosY });
        shapeType->setFillColor({ colorI[0], colorI[1], colorI[2] });
        std::cout << name << " || " << int(colorI[0]) << " || " << int(colorI[1]) << " || " << int(colorI[2]) << '\n';
        std::cout << name << " || " << colorF[0] << " || " << colorF[1] << " || " << colorF[2] << '\n';
    }
    ~GameShape() {
        std::cout << "Called Shape destructor\n";
        delete shapeType;
        shapeType = nullptr;
        delete shapeText;
        shapeText = nullptr;
    };

    void colorItoF() {
        colorF[0] = float(colorI[0]) / 255.0f;
        colorF[1] = float(colorI[1]) / 255.0f;
        colorF[2] = float(colorI[2]) / 255.0f;
    }
    void colorFtoI() {
        colorI[0] = uint8_t(colorF[0] * 255);
        colorI[1] = uint8_t(colorF[1] * 255);
        colorI[2] = uint8_t(colorF[2] * 255);
    }
    void updateShape() {
        shapeType->setScale({scaler, scaler});
        colorFtoI();
        shapeType->setFillColor({ colorI[0], colorI[1], colorI[2] });
    }
    void moveCalc(unsigned int width, unsigned int height) {
        if(shapeType->getPosition().x >= width || shapeType->getPosition().x <= 0){
            speedX = -speedX;
        }
        if (shapeType->getPosition().y >= height || shapeType->getPosition().y <= 0) {
            speedY = -speedY;
        }
        shapeType->setPosition({ currentPosX + speedX, currentPosY +speedY});
        shapeText->setPosition(shapeType->getPosition());
        currentPosX = shapeType->getPosition().x;
        currentPosY = shapeType->getPosition().y;
        shapeText->setPosition({
            shapeType->getPosition().x - (shapeText->getLocalBounds().size.x / 2),
            shapeType->getPosition().y - (shapeText->getLocalBounds().size.y / 2)
            });
    }
    void textInit(sf::Font* sfFont) {
        shapeText = new sf::Text(*sfFont);
        shapeText->setString(name);
        shapeText->setCharacterSize(16);
        shapeText->setFillColor(sf::Color::White);
        shapeText->setPosition({ 
            shapeType->getPosition().x - (shapeText->getLocalBounds().size.x / 2),
            shapeType->getPosition().y - (shapeText->getLocalBounds().size.y / 2)
            });
    }
    void updateText(std::string newName) {
        shapeText->setString(newName);
    }
};

//function declaration
void pollEvent(sf::RenderWindow& window);
void loadFromFile(const char* filename, std::vector<GameShape*>& shapeVector, unsigned int& windowWidth, unsigned int& windowHeight, sf::Font& font, uint8_t* rgb, uint8_t& fontSize);

//Main
int main() {
    
    sf::Font mainFont;
    uint8_t fontColor[3]{};
    uint8_t fontSize;
    std::vector<GameShape*> shapesInGame;
    static char nameBuffer[128];
    static char bottomTextBuffer[128];

    unsigned int width = 1280;
    unsigned int height = 760;

    loadFromFile("config/config.txt", shapesInGame, width, height, mainFont, fontColor, fontSize);
    for (auto s : shapesInGame) {
        s->textInit(&mainFont);
    }

    sf::RenderWindow window(sf::VideoMode({ width, height }), "BouncyShapes");
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window)) {
        std::cout << "Could not initialize window \n";
        std::exit(-1);
    }

    sf::Clock deltaClock;

    //bottom text setup
    sf::Text bottomText(mainFont);
    bottomText.setString("Bottom Text Here");
    bottomText.setCharacterSize(fontSize);
    bottomText.setFillColor({ fontColor[0], fontColor[1], fontColor[2] });
    bottomText.setPosition({ 0, height - (bottomText.getGlobalBounds().size.y * 2) });


    //scale the imgui ui and text size
    ImGui::GetStyle().ScaleAllSizes(1.0f);
    ImGui::GetIO().FontGlobalScale = 1.0f;

    //create the vector for the combo box and add the shapes
    std::vector<const char*> items;
    for (auto& s : shapesInGame) { items.push_back(s->name.c_str()); }
    int itemIndex = 0;

    //Debug stuff
    sf::RectangleShape shapeO({ 50,50 });
    shapeO.setPosition({ float(width) / 2.0f, float(height) / 2.0f });
    shapeO.setFillColor({170, 0, 170});
    //Debug end


    while (window.isOpen()) {
        //Event handling
        pollEvent(window);

        //Physics Update
        for (auto s : shapesInGame) {
            s->updateShape();
            s->moveCalc(width, height);
        }

        //Update ImGui
        ImGui::SFML::Update(window, deltaClock.restart());

        //Draw UI
        ImGui::Begin("Shape Controls");
        ImGui::Combo("Name", &itemIndex, items.data(), (int)items.size());
        ImGui::Checkbox("draw", &shapesInGame[itemIndex]->draw);
        ImGui::SliderFloat("Scale", &shapesInGame[itemIndex]->scaler,0.0f, 4.0f);
        ImGui::SliderFloat("SpeedX", { &shapesInGame[itemIndex]->speedX}, -8.0f, 8.0f);
        ImGui::SliderFloat("SpeedY", { &shapesInGame[itemIndex]->speedY }, -8.0f, 8.0f);
        ImGui::ColorEdit3("Color", shapesInGame[itemIndex]->colorF);

        ImGui::InputText("shapeName", nameBuffer, 128);
        ImGui::SameLine();
        if (ImGui::Button("Change Shape Name")) {
            shapesInGame[itemIndex]->updateText(nameBuffer);
        }

        ImGui::InputText("bottomText", bottomTextBuffer, 128);
        ImGui::SameLine();
        if (ImGui::Button("Change Bottom Text")) {
            bottomText.setString(bottomTextBuffer);
        }
        ImGui::End();

        //Clear Window
        window.clear();
        //Draw window
        for (auto s : shapesInGame) {
            if (s->draw) {
                window.draw(*s->shapeType);
                window.draw(*s->shapeText);
            }
        }
        window.draw(bottomText);
        window.draw(shapeO);
        
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

void loadFromFile(const char* filename, std::vector<GameShape*>& shapeVector, unsigned int& windowWidth, unsigned int& windowHeight, sf::Font& font, uint8_t* rgb, uint8_t& fontSize) {
    std::ifstream fin(filename);
    std::string temp;

    while (fin >> temp){
        if (temp == "Window") {
            fin >> windowWidth >> windowHeight;
        }
        else if (temp == "Font") {
            std::string fontBuffer;
            int sizeBuffer;
            int rBuffer;
            int gBuffer;
            int bBuffer;
            fin >> fontBuffer >> sizeBuffer >> rBuffer >> gBuffer >> bBuffer;
            rgb[0] = uint8_t(rBuffer);
            rgb[1] = uint8_t(gBuffer);
            rgb[2] = uint8_t(bBuffer);
            fontSize = uint8_t(sizeBuffer);
            //std::cout << fontBuffer << "Font Color: " << sizeBuffer << int(rgb[0]) << int(rgb[1]) << int(rgb[2]) << '\n';
            if (!font.openFromFile(fontBuffer.c_str()))
            {
                std::cout << "COULD NOT LOAD FONT" << fontBuffer << '\n';
            }
        }
        else if (temp == "Circle") {
            std::string name;
            float posX;
            float posY;
            float speedX;
            float speedY;
            int r;
            int g;
            int b;
            float radius;
            fin >> name >> posX >> posY >> speedX >> speedY >> r >> g >> b >> radius;
            shapeVector.push_back(new GameShape(name, posX, posY, speedX, speedY, r, g, b, radius));
            //std::cout << name << " || " << r << " || " << g << " || " << b << '\n';
        }
        else if (temp == "Rectange") {
            std::string name;
            float posX;
            float posY;
            float speedX;
            float speedY;
            int r;
            int g;
            int b;
            float width;
            float height;
            fin >> name >> posX >> posY >> speedX >> speedY >> r >> g >> b >> width >> height;
            shapeVector.push_back(new GameShape(name, posX, posY, speedX, speedY, r, g, b, width, height));
            //std::cout << name << " || " << r << " || " << g << " || " << b << '\n';
        }
    }
}

/*TO DO IMPROVEMENT IDEAS:
* add unique IDs to the shapes. having two shapes with the same name will lead to bugs
* 
*/