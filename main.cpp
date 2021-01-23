#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <bits/stdc++.h>
#include <Windows.h>
using namespace sf;
using namespace std;
int size = 56;
Vector2f offset(28, 28);
Sprite f[32];
string position = "";
int board[8][8] = 
{-1,-2,-3,-4,-5,-3,-2,-1,
 -6,-6,-6,-6,-6,-6,-6,-6,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  6, 6, 6, 6, 6, 6, 6, 6,
  1, 2, 3, 4, 5, 3, 2, 1
};
string toChessNote(Vector2f p)
{
	string s = "";
	s += char(p.x / size + 97);
	s += char(7 - p.y / size + 49);
	return s;
}
Vector2f toCoord(char a, char b)
{
	int x = int(a) - 97;
	int y = 7 - int(b) + 49;
	return Vector2f(x * size, y * size);
}
void move(string str)
{
    Vector2f oldPos = toCoord(str[0], str[1]);
    Vector2f newPos = toCoord(str[2], str[3]);
    for(int i = 0; i < 32; i++)
    	if (f[i].getPosition() == newPos) f[i].setPosition(-100, -100);
    for(int i = 0; i < 32; i++)
    	if (f[i].getPosition()==oldPos) f[i].setPosition(newPos);
    if (str == "e1g1") if (position.find("e1") == -1) move("h1f1"); 
    if (str == "e8g8") if (position.find("e8") == -1) move("h8f8");
    if (str == "e1c1") if (position.find("e1") == -1) move("a1d1");
    if (str == "e8c8") if (position.find("e8") == -1) move("a8d8");
}
void loadPosition()
{
    int k = 0;
    for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++)
    {
    	int n = board[i][j];
    	if (!n) continue;
    	int x = abs(n) - 1;
    	int y = n > 0 ? 1 : 0;
    	f[k].setTextureRect(IntRect(size * x, size * y, size, size));
    	f[k].setPosition(size * j, size * i);
    	k++;
    }
    for(int i = 0; i < position.length(); i += 5)
    	move(position.substr(i, 4));
}
int main()
{
    RenderWindow window(VideoMode(504, 504), "Consciousness Chess!", Style::Close);
	Music msp;
	msp.openFromFile("a.wav");
	bool playG = false;
	msp.play();
	while(window.isOpen())
	{
		Event e;
		while(window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
        		if (MessageBox(NULL, TEXT("Are you sure?"), TEXT("Consciousness Chess!"), MB_YESNO | MB_ICONQUESTION) == IDYES)
				{
            	    window.close();
            	    return 0;
            	}
        	}
        	if (e.type == Event::Closed)
            	window.close();
		}
		Texture play, quit, bt;
		bt.loadFromFile("images/St.png");
		play.loadFromFile("images/play.png");
		quit.loadFromFile("images/quit.png");
		Sprite play1(play), quit1(quit), bt1(bt);
		play1.setPosition(32, 380);
		quit1.setPosition(355, 380);
    	window.draw(bt1);
		window.draw(play1);
		window.draw(quit1);
		FloatRect rectMouse((float)Mouse::getPosition(window).x, (float)Mouse::getPosition(window).y, 0.0001f, 0.0001f);
		if(quit1.getGlobalBounds().intersects(rectMouse))
		{
			if(Mouse::isButtonPressed(Mouse::Left)) return 0;
		}
		if(play1.getGlobalBounds().intersects(rectMouse))
		{
			if(Mouse::isButtonPressed(Mouse::Left))
			{
				playG = true;
				break;
			}
		}
		window.display();
	}
		if(playG == true)
		{
    		Texture t1, t2;
    		t1.loadFromFile("images/figures.png"); 
    		t2.loadFromFile("images/board.png");
    		for(int i = 0; i < 32; i++) f[i].setTexture(t1);
    		Sprite sBoard(t2); 
    		loadPosition();
    		bool isMove = false;
    		float dx = 0, dy = 0;
    		Vector2f oldPos, newPos;
    		string str;
    		int n = 0; 
    		while (window.isOpen())
    		{
        		Vector2i pos = Mouse::getPosition(window) - Vector2i(offset);
        		Event e;
        		while (window.pollEvent(e))
        		{
        			if (e.type == Event::Closed)
					{
                    	if (MessageBox(NULL, TEXT("Are you sure?"), TEXT("Consciousness Chess!"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        	window.close();
                        	return 0;
                    	}
                	}
            		if (e.type == Event::Closed)
            		    window.close();
            		if (e.type == Event::KeyPressed)
            	    	if (e.key.code == Keyboard::BackSpace)
            	    	{ if (position.length() > 6) position.erase(position.length() - 6, 5); loadPosition();}
            		if (e.type == Event::MouseButtonPressed)
                		if (e.key.code == Mouse::Left)
                			for(int i = 0; i < 32; i++)
                				if (f[i].getGlobalBounds().contains(pos.x,pos.y))
                    			{
                    				isMove = true; n = i;
                    				dx = pos.x - f[i].getPosition().x;
                    				dy = pos.y - f[i].getPosition().y;
                    				oldPos = f[i].getPosition();
                    			}
            		if (e.type == Event::MouseButtonReleased)
                		if (e.key.code == Mouse::Left)
                		{
                			isMove = false;
                			Vector2f p = f[n].getPosition() + Vector2f(size / 2, size / 2);
                			newPos = Vector2f( size * int(p.x / size), size * int(p.y / size) );
                			str = toChessNote(oldPos) + toChessNote(newPos);
                			move(str); 
                			if (oldPos != newPos) position += str + " ";
                			f[n].setPosition(newPos);                   
                		}                       
        		}
    			if (Keyboard::isKeyPressed(Keyboard::Space))
    			{
//        			str = getNextMove(position);
        			oldPos = toCoord(str[0], str[1]);
        			newPos = toCoord(str[2], str[3]);
        			for(int i = 0; i < 32; i++) if (f[i].getPosition() == oldPos) n = i;
        			for(int k = 0; k < 50; k++)
        			{
            			Vector2f p = newPos - oldPos;
            			f[n].move(p.x / 50, p.y / 50); 
            			window.draw(sBoard);
            			for(int i = 0; i < 32; i++) f[i].move(offset);
            			for(int i = 0; i < 32; i++) window.draw(f[i]); window.draw(f[n]);
            			for(int i = 0; i < 32; i++) f[i].move(-offset);
            			window.display();
        			}
        			move(str);
					position += str + " ";
        			f[n].setPosition(newPos); 
        		}
        		if (isMove) f[n].setPosition(pos.x - dx, pos.y - dy);
    			window.clear();
    			window.draw(sBoard);
    			for(int i = 0; i < 32; i++) f[i].move(offset);
    			for(int i = 0; i < 32; i++) window.draw(f[i]); window.draw(f[n]);
    			for(int i = 0; i < 32; i++) f[i].move(-offset);
    			window.display();
    		}
    	}
    return 0;
}
