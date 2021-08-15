#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <SFML/Window.hpp>
#include <math.h>
#include <cmath>                                         // для функции cos
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <sstream>

#define PI 3.14159265     

using namespace std;

const int FPS = 60;
int width;
int height;
int x;
int y;
int r;
float moving_line_angle;
float moving_line_step;
sf::RenderWindow App(sf::VideoMode(width, height), "SFML Graphics", sf::Style::Fullscreen);
sf::Vertex line[2];
sf::Vertex UnderLine[2];
vector<pair<float,float>> Targets;
vector<pair<float,float>> Founded_Targets;
sf::RectangleShape Rect;

void Setup(){
    //App.setFramerateLimit(FPS);
    width = App.getSize().x;
    height = App.getSize().y;
    x = width / 2;
    y = height - 20;
    r = width / 2 - 25;
    moving_line_angle = 0;
    moving_line_step = 0.1;
    line[0].position = sf::Vector2f(x, y);
    line[0].color = sf::Color(0,255,0);
    line[1].color = sf::Color(0,255,0);
    UnderLine[0].position = sf::Vector2f(x - r, y + 8);
    UnderLine[1].position = sf::Vector2f(x + r, y + 8);
    UnderLine[0].color = sf::Color::Yellow;
    UnderLine[1].color = sf::Color::Yellow;
    Rect.setSize(sf::Vector2f(width, height));
    Rect.setFillColor(sf::Color(0,0,0,6));

    for (int i = 0; i < 50; i++){
        pair<float,float> Pos;
        Pos.first = rand() % width;
        Pos.second = rand() % height;
        Targets.push_back(Pos);
    }
}

void DrawText(string Text, int x, int y, sf::Color myColor){
    sf::Text text;
    sf::Font font;
    font.loadFromFile("Roboto-Bold.ttf");
    text.setFont(font);
    text.setFillColor(myColor);
    text.setCharacterSize(35);
    text.setString(Text);
    text.move(x, y);
    App.draw(text);    
}

float CalcDist(float xf, float yf, float xl, float yl){
    return sqrt(pow((xf - xl), 2) + pow((yf - yl), 2));
}

void Update(){
    moving_line_angle += moving_line_step;
    if(moving_line_angle >= 180 || moving_line_angle <= 0){
        moving_line_step = -moving_line_step;
        Founded_Targets.clear();
    }
    line[1].position = sf::Vector2f(x + r * cos(moving_line_angle * PI / 180), y - r * sin(moving_line_angle * PI / 180));
    
    for(int i = 0; i <= Targets.size(); i++){
        float Dist = CalcDist(x,y,Targets[i].first, Targets[i].second);

        if(Targets[i].first == floor(x + cos(moving_line_angle * PI / 180) * Dist)){
            if(Dist < r && Targets[i].second <= y){ 
                Founded_Targets.push_back(Targets[i]);
            }
        }
    }
}

void Render(){    
    App.draw(Rect);
    App.draw(UnderLine, 2, sf::PrimitiveType::LinesStrip);
    for(int i = 0; i <= 180; i+=30){
        float a = x - 17.5 + (r + 30) * cos(i * PI / 180); 
        float b = y - 40 - (r + 30) * sin(i * PI / 180);   
        DrawText(to_string(i), a, b, sf::Color::Red); 
    }

    for (int i = 0; i <= 180; i++){
        sf::CircleShape Circle(3);
        Circle.setFillColor(sf::Color::Yellow);    
        float a = x + r * cos(i * PI / 180); 
        float b = y - r * sin(i * PI / 180);
        Circle.move(a,b);
        App.draw(Circle);
    }

    for(int i = 0; i < Founded_Targets.size(); i++){
        sf::CircleShape Target(10);
        Target.setFillColor(sf::Color::Red);
        Target.move(Founded_Targets[i].first, Founded_Targets[i].second);
        //DrawText("(" + to_string(int(Founded_Targets[i].first)) + "," + to_string(int(Founded_Targets[i].second)) + ")", Founded_Targets[i].first + 20, Founded_Targets[i].second - 50, sf::Color::White);
        App.draw(Target);   
    }

    App.draw(line, 2, sf::PrimitiveType::LinesStrip);
    App.display();
}

int main(){ 
    srand(time(0));
    Setup();
    // Start game loop
    while (App.isOpen())
    {
        // Process events
        sf::Event Event;
        while (App.pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                App.close();
        }
        Update();
        Render();
    }
}
