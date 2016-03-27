#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <math.h>
#include <fstream>

class Level{
	public:
		struct fallInfo{
			bool falling;
			int topOfTile;	
		};
		struct usedLvlTiles{
			int type, rightTop, leftTop;
			bool lift;
			sf::Texture tex;
			std::string loc;
		};
		struct tileInfo{
			int type, rightTop, leftTop, tileX, tileY;
			bool lift;
			int floorHeight(int x, int y){
				int topOfTile = 0;
				fallInfo fallInfo1;
				float player = (x - tileX/32);
				topOfTile = tileY-leftTop - ((-player*leftTop) + (player*rightTop));
				return topOfTile;
			}
		};
		
		
		sf::Texture plat;
		sf::Texture ramp1;
		sf::Texture ramp2;
		sf::Texture ramp3;
		sf::Texture ramp4;
		sf::Texture tBackground;
		sf::Sprite background;
		
		sf::View playerView;
		
		int lvlArrayX, lvlArrayY, lvlEnd;
		tileInfo lvlArray[200][40];
		usedLvlTiles usedTiles[20];
		std::vector<sf::Sprite> tileArray;
		fallInfo fallInfo1;
		
		sf::Texture loadImage(std::string loc){
			sf::Image img;
			img.loadFromFile(loc);
			img.createMaskFromColor(sf::Color(127,0,0));
			sf::Texture tex;
			tex.loadFromImage(img);
			return tex;
		}
		
		Level(){
			/*plat = loadImage("Images/plat.bmp");
			ramp1 = loadImage("Images/ramp1.bmp");
			ramp2 = loadImage("Images/ramp2.bmp");
			ramp3 = loadImage("Images/ramp3.bmp");
			ramp4 = loadImage("Images/ramp4.bmp");*/
			tBackground.loadFromFile("Images/background.bmp");
			background.setTexture(tBackground);
			background.setScale(1.5f,1.5f);
			playerView.setSize(1280,720);
			playerView.setCenter(640,360);
			lvlArrayX = 128;
			lvlArrayY = 20;
			lvlEnd = lvlArrayX * 32;
			loadLevel("sample.lvl");
		}
		
		void loadTile(usedLvlTiles& tile, int arrayX, int arrayY){
			sf::Texture tempTex;
			sf::Sprite tempSpr;
			tempSpr.setPosition((arrayX*32),(arrayY*32));
			tempSpr.setTexture(tile.tex);
			lvlArray[arrayX][arrayY].type = tile.type;
			lvlArray[arrayX][arrayY].lift = tile.lift;
			lvlArray[arrayX][arrayY].tileX = arrayX*32;
			lvlArray[arrayX][arrayY].tileY = arrayY*32;
			lvlArray[arrayX][arrayY].leftTop = tile.leftTop;
			lvlArray[arrayX][arrayY].rightTop = tile.rightTop;
			if(tile.type != 0){
				tileArray.push_back(tempSpr);
			}
		}
		
		void loadLevel(std::string loc){
			std::ifstream file;
			file.open(loc.c_str());
			std::string line = "";
			std::string tempStr = "";
			int tileNum = 0, arrayX = 0, arrayY = 0;
			bool loadArray = false;
			while(std::getline(file,line)){
				int c = 0;
				for(int i = 0; i < line.length(); ++i){
					char tempChar = line.at(i);
					if(!loadArray){
						if(tempChar == '|'){
							loadArray = true;
						}
						else if(tempChar != ':' && tempChar != ',' && tempChar != ';'){
							tempStr += tempChar;
						}							
						else{
							switch(c){
								case 0:
									tileNum = atoi(tempStr.c_str());
									break;
								case 1:
									usedTiles[tileNum].type = atoi(tempStr.c_str());
									break;
								case 2:
									usedTiles[tileNum].lift = atoi(tempStr.c_str());
									break;
								case 3:
									usedTiles[tileNum].leftTop = atoi(tempStr.c_str());
									break;
								case 4:
									usedTiles[tileNum].rightTop = atoi(tempStr.c_str());
									break;
								case 5:
									usedTiles[tileNum].tex = loadImage(tempStr);
									++tileNum;
									break;
							}
							tempStr = "";
							++c;
						}
					}
					else{
						if (tempChar != ',' && tempChar != ';'){
							tempStr += tempChar;
						}
						else{
							loadTile(usedTiles[atoi(tempStr.c_str())], arrayX, arrayY);
							tempStr = "";
							++arrayX;
						}
					}
				}
				if(loadArray){
					arrayX = 0;
					++arrayY;
				}
				++tileNum;
			}
		}
		
		void drawLevel(sf::RenderWindow& App, sf::Sprite spr){
			App.draw(background);
			for(int i = 0;i < tileArray.size()-1; ++i){
				App.draw(tileArray.at(i));
			}
		}
		fallInfo checkHit(sf::Sprite& spr){
			fallInfo temp;
			int x = spr.getPosition().x;
			int y = spr.getPosition().y;
			sf::Vector2<int> guyTile(x/32,y/32);
			if(lvlArray[guyTile.x][guyTile.y].type == 0){
				temp.falling = true;
				temp.topOfTile = -371;
				if(lvlArray[guyTile.x-1][guyTile.y].type == 1){
					temp.falling = false;
					temp.topOfTile = lvlArray[guyTile.x-1][guyTile.y].floorHeight(x,y);
				}
				else if(lvlArray[guyTile.x+1][guyTile.y].type == 1){
					temp.falling = false;
					temp.topOfTile = lvlArray[guyTile.x+1][guyTile.y].floorHeight(x,y);
				}
			}
			else if(lvlArray[guyTile.x][guyTile.y].type == 1){
				temp.falling = false;
				if(lvlArray[guyTile.x][guyTile.y-1].lift){
					temp.topOfTile = lvlArray[guyTile.x][guyTile.y-1].floorHeight(x,y);
				}
				else{
					temp.topOfTile = lvlArray[guyTile.x][guyTile.y].floorHeight(x,y);
				}
			}
			return temp;
		}
		
		void updateView(sf::Sprite& p1, float e){
			int x = p1.getPosition().x, y = p1.getPosition().y, x2 = playerView.getCenter().x, y2 = playerView.getCenter().y;
			if(x-50 > x2){
				x2= x-50;
			}
			if(x+50 < x2){
				x2= x+50;
			}
			if(y-250 > y2){
				y2= y-250;
			}
			if(y+250 < y2){
				y2= y+250;
			}
			playerView.setCenter(x2,y2);
		}
	private:
};

#endif
