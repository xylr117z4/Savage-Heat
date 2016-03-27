#define SFML_STATIC
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Level.hpp"
#include "Player.hpp"
#include "bulletTracker.hpp"

//This 'ill be my full time job, fuck businessmen with their high horses.

sf::Texture loadImage(std::string loc){
	sf::Image img;
	img.loadFromFile(loc);
	img.createMaskFromColor(sf::Color(127,0,0));
	sf::Texture tex;
	tex.loadFromImage(img);
	return tex;
}

int main(int argc, char** argv) {
	
	sf::RenderWindow App(sf::VideoMode(1280,720),"Savage Heat: A bit Harder than the Corps");
	App.setFramerateLimit(60);
	//App.setVerticalSyncEnabled(true);
	BulletTracker bulletList;
	sf::Texture tex = loadImage("Images/Player1.bmp");
	sf::Texture tex2 = loadImage("Images/Bullet2.bmp");
	sf::Clock clock;
	Level disLevel;
	Player p1(tex,tex2);
	p1.name = "Player 1";
	
	
	while(App.isOpen()){
		sf::Event event;
		sf::Time time = clock.restart();
		float elapsed = time.asSeconds();
		
		while(App.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				App.close();
			}
			if(event.type == sf::Event::KeyPressed){
				if(event.key.code == sf::Keyboard::F11){
					
				}
				if(event.key.code == sf::Keyboard::Escape){
					App.close();
				}
			}
		}
		
		p1.update((1.f/60.f)/*elapsed*/, disLevel, bulletList);
		bulletList.update((1.f/60.f)/*elapsed*/);
		disLevel.updateView(p1.Sprite, (1.f/60.f)/*elapsed*/);
		App.setView(disLevel.playerView);
		App.clear();
		disLevel.drawLevel(App,p1.Sprite);
		bulletList.draw(App);
		App.draw(p1.Sprite);
		App.display();
	}	
}
