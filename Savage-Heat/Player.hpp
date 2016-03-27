#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Level.hpp"
#include "BulletTracker.hpp"


/*
I'd say the best course of action would be just add DIR as an array then have two directions be set, so the first forward or backward then the second will be up/down/mid.
*/

class Player{
	public:
		enum dir{
			forward = 0, backward = 1, normal = 2, upward = 3, downward = 4
		};
		struct control{
			sf::Keyboard::Key up,down,left,right,jump,shoot;
		};
		struct action{
			bool shooting, moving;			
			dir direction[2];
		};
		
		int health,speed;
		std::string name;
		float jumpSpeed,fallSpeed, jumpTime, frameTime, shotTime;
		bool jumped, again, falling;
		sf::Sprite Sprite;
		sf::Texture tBullet; 
		sf::SoundBuffer bShot;
		sf::Sound Shot;
		int showingSprite, showingFrame;
		control controls;
		action actions;
		
		Player(sf::Texture &tex, sf::Texture &bulTex){
			health = 100;
			name = "Player x";
			showingSprite = 0;
			speed = 350;
			jumpSpeed = 0;
			jumpTime = 0;
			fallSpeed = 30;
			jumped = false;
			again = true;
			frameTime = 0;
			tBullet = bulTex;
			sf::Sprite spr(tex);
			Sprite = spr;
			Sprite.setTextureRect(sf::Rect<int>(0,0,106,136));
			Sprite.setOrigin(Sprite.getTextureRect().width/2, Sprite.getTextureRect().height);
			Sprite.setPosition(60,200);
			bShot.loadFromFile("Sounds/Shot.wav");
			Shot.setBuffer(bShot);
			controls.up = sf::Keyboard::W;
			controls.down = sf::Keyboard::S;
			controls.left = sf::Keyboard::A;
			controls.right = sf::Keyboard::D;
			controls.jump = sf::Keyboard::Space;
			controls.shoot = sf::Keyboard::J;
		}
		
		void animate(float e, bool r, bool j){ //elapsed, reset, jumped
			int f = 10;
			int frameY;
			int frameSizeX = 113, frameSizeY = 136;
			
			frameY = 0;
			
			if(actions.direction[1] == upward){
				frameY = frameSizeY*2;
				if(actions.moving){
					frameY = frameSizeY*4;
				}
			}
			
			if(j){
				r = false;
				f = 6;
				frameY = frameSizeY*8+1;
			}
			
			else if(actions.shooting){
				frameY = frameY + frameSizeY;
			}
			
			if(actions.shooting){
				if(shotTime < 15){
					shotTime += e*100;
					if(shotTime > 7 && !j){
						frameY -= frameSizeY;
					}
				}
				else{
					shotTime = 0;
					actions.shooting = false;
				}
			}
			
			if(r){
				frameTime = 0;
				Sprite.setTextureRect(sf::Rect<int>(0,frameY,106,136));
			}
			else{
				frameTime += e*100;
				if(frameTime < f*1){
					Sprite.setTextureRect(sf::Rect<int>(frameSizeX,frameY,frameSizeX,frameSizeY));
				}
				else if(frameTime < f*2){
					Sprite.setTextureRect(sf::Rect<int>(frameSizeX*2,frameY,frameSizeX,frameSizeY));
				}
				else if(frameTime < f*3){
					Sprite.setTextureRect(sf::Rect<int>(frameSizeX*3,frameY,frameSizeX,frameSizeY));
				}
				else if(frameTime < f*4){
					Sprite.setTextureRect(sf::Rect<int>(frameSizeX*4,frameY,frameSizeX,frameSizeY));
				}
				else if(frameTime < f*5){
					Sprite.setTextureRect(sf::Rect<int>(frameSizeX*5,frameY,frameSizeX,frameSizeY));
				}
				else if(frameTime < f*6){
					Sprite.setTextureRect(sf::Rect<int>(frameSizeX*6,frameY,frameSizeX,frameSizeY));
				}
				else if(frameTime < f*7 && j){
					Sprite.setTextureRect(sf::Rect<int>(frameSizeX*7,frameY,frameSizeX,frameSizeY));
				}
				else{
					frameTime = 0;
				}
				
				if(actions.direction[0] == forward){
					Sprite.setScale(1.0,1.0);
				}
				else if (actions.direction[0] == backward){
					Sprite.setScale(-1.0,1.0);
				}
			}
		}
		
		void update(float e, Level& lvl, BulletTracker& Bullets){
			bool reset = true;
			int s = 5;
			actions.moving = false;
			if(sf::Keyboard::isKeyPressed(controls.right)){
				actions.direction[0] = forward;
				actions.moving = true;
				Sprite.move(speed*e,0);
				reset = false;
			}
			else if(sf::Keyboard::isKeyPressed(controls.left)){
				actions.direction[0] = backward;
				actions.moving = true;
				Sprite.move(-speed*e,0);
				reset = false;
			}
			
			if(sf::Keyboard::isKeyPressed(controls.up)){
				actions.direction[1] = upward;
			}			
			else if(sf::Keyboard::isKeyPressed(controls.down)){
				actions.direction[1] = downward;
			}
			else{
				actions.direction[1] = normal;
			}
			
			if(sf::Keyboard::isKeyPressed(controls.shoot) && !actions.shooting){
				if(actions.direction[1] == upward){
					if(actions.moving){
						if(actions.direction[0] == forward){
							Bullets.fireNew(name, tBullet, Sprite, 1300, 145, sf::Vector2f(0,-80), 5);
						}
						else if(actions.direction[0] == backward){
							Bullets.fireNew(name, tBullet, Sprite, 1300, 35, sf::Vector2f(0,-80), 4);
						}
					}
					else{
						if(actions.direction[0] == forward){
							Bullets.fireNew(name, tBullet, Sprite, 1300, 90, sf::Vector2f(-17,-75), 3);
						}
						else if(actions.direction[0] == backward){
							Bullets.fireNew(name, tBullet, Sprite, 1300, 90, sf::Vector2f(-2,-75), 2);
						}
					}
				}
				else if(actions.direction[0] == forward){
					if(actions.moving){
						Bullets.fireNew(name, tBullet, Sprite, speed+1300, 180, sf::Vector2f(0,-60), 0);
					}
					else{
						Bullets.fireNew(name, tBullet, Sprite, 1300, 180, sf::Vector2f(0,-60), 0);
					}
				}
				else if(actions.direction[0] == backward){
					if(actions.moving){
						Bullets.fireNew(name, tBullet, Sprite, speed+1300, 0, sf::Vector2f(0,-60), 1);
					}
					else{
						Bullets.fireNew(name, tBullet, Sprite, 1300, 0, sf::Vector2f(0,-60), 1);
					}
				}
				//Shot.play();
				actions.shooting = true;
			}
			
			animate(e, reset, fall(e, sf::Keyboard::isKeyPressed(controls.jump), lvl));
			
		}
		
		bool fall(float e, bool jumpPressed, Level& lvl){
			if(jumpPressed && jumpTime < 0.20 && again){
				if(!jumped){
					jumped = true;
					jumpSpeed = -500;
				}
				Sprite.move(0,jumpSpeed*e);
				jumpTime += e;
			}
			else{
				if(jumped){
					again = false;
				}
				Level::fallInfo falling = lvl.checkHit(Sprite);
				int x = Sprite.getPosition().x;
				int y = Sprite.getPosition().y;
				if(!falling.falling && jumpSpeed > -1){
					Sprite.setPosition(x,falling.topOfTile);
					if(!jumpPressed){
						again = true;
					}
					jumpSpeed = 0;
					jumpTime = 0;
					jumped = false;
				}
				else{
					jumpSpeed += fallSpeed;
					Sprite.move(0,jumpSpeed*e);
				}
			}
			
			return jumped;
		}
	private:
};

#endif
