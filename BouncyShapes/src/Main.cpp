#include<iostream>
#include<fstream>
#include<memory>

#include "imgui.h"
#include "imgui-SFML.h"

#include<SFML/Graphics.hpp>

void pollEvent(sf::RenderWindow& window);
void loadFromFile(const char* filename);

int main() {

    unsigned int width = 1280;
    unsigned int height = 760;

    sf::RenderWindow window(sf::VideoMode({ width, height }), "BouncyShapes");
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window)) {
        std::cout << "Could not initialize window \n";
        std::exit(-1);
    }

    sf::Clock deltaClock;

    loadFromFile("config/config.txt");

    //scale the imgui ui and text size by 2
    ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::GetIO().FontGlobalScale = 2.0f;

    while (window.isOpen()) {
        pollEvent(window);

        //Update ImGui
        ImGui::SFML::Update(window, deltaClock.restart());

        //Draw UI
        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        //Clear Window
        window.clear();
        //Draw window
        ImGui::SFML::Render(window);
        //Display Window
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

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

void loadFromFile(const char* filename) {
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
            std::cout << "A circle \n";
        }
        else if (temp == "Rectange") {
            std::cout << "A Rectangle \n";
        }
    }

}