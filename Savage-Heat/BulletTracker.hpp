#ifndef BulletTracker_HPP
#define BulletTracker_HPP
#include <cmath>
#include "Player.hpp"

class BulletTracker{
	
	struct Bullet{
		sf::Sprite Sprite;
		double FrameTime;
		int Speed, Face;
		double Direction;
		std::string Owner;
		bool Active, Reset;	
	};
	
	public:	
		Bullet Bullets[128];
		int bulletNumber;
		
		BulletTracker(){
			sf::Texture bulTex;
			bulletNumber = 0;
			for(int i = 0; i < 128; ++i){
				Bullets[i].Active = false;
			}
		}
		int findInactive(){
			for(int i = 0; i < 128; ++i){
				if(!Bullets[i].Active){
					return i;
				}
			}
			return -1;
		}
		void fireNew(std::string playerName, sf::Texture &tBullet, sf::Sprite &playerSprite, int speed, double angle, sf::Vector2f origin, int face){
			int i = findInactive();
			if(i != -1){
				Bullets[i].Active = true;
				Bullets[i].Reset = true;
				Bullets[i].Face = face;
				Bullets[i].Owner = playerName;
				Bullets[i].Sprite.setTexture(tBullet);
				Bullets[i].Sprite.setPosition(playerSprite.getPosition().x+origin.x, playerSprite.getPosition().y+origin.y);
				Bullets[i].Sprite.setOrigin(0,0);
				Bullets[i].Speed = speed;
				Bullets[i].Direction = angle*(M_PI/180);
				Bullets[i].FrameTime = 0;
				bulletNumber += 1;
			}else{ std::cout << "TOO MANY!!!\n"; }
		}
		void animate(int i, double e){
			int f = 12;
			int frameY = 0;
			int frameSizeX = 19, frameSizeY = 8;
			if(Bullets[i].Face == 1){
				frameY = 8;
			}
			else if(Bullets[i].Face == 2){
				frameY = 16;
				frameSizeX = 19, frameSizeY = 19;
			}
			else if(Bullets[i].Face == 3){
				frameY = 35;
				frameSizeX = 19, frameSizeY = 19;
			}
			else if(Bullets[i].Face == 4){
				frameY = 54;
				frameSizeX = 19, frameSizeY = 19;
			}
			else if(Bullets[i].Face == 5){
				frameY = 73;
				frameSizeX = 19, frameSizeY = 19;
			}
			
			if(Bullets[i].Reset){
				Bullets[i].FrameTime = 0;
				Bullets[i].Reset = false;
				Bullets[i].Sprite.setTextureRect(sf::Rect<int>(0,frameY,19,8));
			}
			else{
				Bullets[i].FrameTime += e*100;
				if(Bullets[i].FrameTime < f*2){
					Bullets[i].Sprite.setTextureRect(sf::Rect<int>(frameSizeX,frameY,frameSizeX,frameSizeY));
				}
				else if(Bullets[i].FrameTime < f*3){
					Bullets[i].Sprite.setTextureRect(sf::Rect<int>(frameSizeX*2,frameY,frameSizeX,frameSizeY));
				}
				else{
					Bullets[i].FrameTime = 0;
					Bullets[i].Reset = true;
				}
			}
		}
		void update(double e){
			//std::cout << "Number of bullets: " << bulletNumber << "\n";
			for(int i = 0; i < bulletNumber; ++i){
				if(Bullets[i].Active){
					Bullets[i].Sprite.move(cos(Bullets[i].Direction) * -Bullets[i].Speed*e, sin(Bullets[i].Direction) * -Bullets[i].Speed*e);
					animate(i, e);
				}
			}
		}
		void draw(sf::RenderWindow& App){
			for(int i = 0; i < 128; ++i){
				if(Bullets[i].Active){
					App.draw(Bullets[i].Sprite);
				}
			}
		}
		void remove(int i){
			Bullets[i].Active = false;
			bulletNumber -= 1;
		}
		void hitCheck(){
			//for player hits
		}
		void wallCheck(Level& disLevel){
			//wall hits
		}
};

#endif
