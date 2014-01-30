#include <libfreenect.hpp>
#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <cmath>
#include <vector>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <list>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SDL/SDL.h>
#include <libfreenect_sync.h>
#include "MyFreenectDevice.hpp"
#include "Coords.hpp"
#include "Button.hpp"
#include "Finger.hpp"

Freenect::Freenect freenect;
MyFreenectDevice* device;
std::list<Finger*> fList;

void drawPixel(int x, int y, SDL_Surface *w, SDL_Surface *p, SDL_Rect pos, int r, int g, int b)
{
  SDL_FillRect(p, NULL, SDL_MapRGB(w->format, r,g, b));
  pos.x = x;
  pos.y = y;
  SDL_BlitSurface(p, NULL, w, &pos);
}

#define FINGER_TIP_THRESHOLD 20

int main(void)
{
  sf::Window App(sf::VideoMode(800, 600, 32), "SFML OpenGL");
  sf::Clock Clock;
  SDL_Surface *w = NULL;
  SDL_Surface *p = NULL, *p2 = NULL;
  SDL_Rect pos;

  p = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, 1, 32, 0, 0, 0, 0);
  p2 = SDL_CreateRGBSurface(SDL_HWSURFACE, 10, 10, 32, 0, 0, 0, 0);
  SDL_Init(SDL_INIT_VIDEO);
  w = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);
  device = &freenect.createDevice<MyFreenectDevice>(0);
  device->startDepth();
  device->startVideo();
  // Set color and depth clear value
  glClearDepth(1.f);
  glClearColor(0.f, 0.f, 0.f, 0.f);
  // Enable Z-buffer read and write
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // glMatrixMode(GL_MODELVIEW);
  gluPerspective(90.f, 1.f, 1.f, 2500.f);  App.SetActive();
  device->setVideoFormat((freenect_video_format)0);
  uint8_t *dep = (uint8_t*)malloc(640 * 480);
  bzero(dep, sizeof(dep));
  int x, y, z;
  x = -250;
  y = 100;
  z = -270;
  int r = 45;
  float rotateY = 0;
  while (App.IsOpened())
    {
      sf::Event Event;
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      
      glTranslatef(x, y, z);
      while (App.GetEvent(Event))
	{
	  if (Event.Type == sf::Event::Closed)
	    {
	      device->stopDepth();
	      device->stopVideo();
	      exit(0);
	    }
	  if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
	    {
	      device->stopDepth();
	      device->stopVideo();
	      exit(0);
	    }
	  if ((Event.Type == sf::Event::KeyPressed))
	    {
	      if (Event.Key.Code == sf::Key::Z)
		{
		  --rotateY;
		}
	      if (Event.Key.Code == sf::Key::X)
		{
		  ++rotateY;
		}
	      if ((Event.Key.Code == sf::Key::Up))
		{
		  r += 10;
		  device->setTiltDegrees(r);
		}
	      if ((Event.Key.Code == sf::Key::Down))
		{
		  r -= 10;
		  device->setTiltDegrees(r);
		}
	      if ((Event.Key.Code == sf::Key::W))
		{z+=10;
		}
	      if ((Event.Key.Code == sf::Key::S))
		{z-=10;
		}
	      if ((Event.Key.Code == sf::Key::A))
		{x+=10;
		}
	      if ((Event.Key.Code == sf::Key::D))
		{x-=10;}
	      if ((Event.Key.Code == sf::Key::E))
		{y+=10;}
	      if ((Event.Key.Code == sf::Key::Q))
		{y-=10;}
	    }
	}
      if (device->m_new_rgb_frame && device->m_new_depth_frame)
	{
	  int x = 0, y = 0;
	  glRotated(180, 1, 0, 0);
	  glBegin(GL_POINTS);
	  // while (y < 480)
	  //   {
	  //     x = 0;
	  //     while (x < 640)
	  // 	{
	  // 	  device->matrixBin[y][x] = 0;
	  // 	  if (device->xyzCoordsCurr[y][x][2] < device->xyzCoords[y][x][2] - 4
	  // 	      && device->xyzCoordsCurr[y][x][2] < 400)
	  // 	    device->matrixBin[y][x] = 1;
	  // 	  ++x;
	  // 	}
	  //     ++y;
	  //   }
	  y = 0;
	  // while (y < 480)
	  //   {
	  //     x = 0;
	  //     while (x < 640)
	  // 	{
	  // 	  int tmp = 1;
	  // 	  while (tmp < 20)
	  // 	    {
	  // 	      if (device->matrixBin[y][x] == 1
	  // 		  && device->matrixBin[y][x - tmp] == 1
	  // 		  && device->matrixBin[y][x + tmp] == 1)
	  // 		{
	  // 		  device->matrixBin[y][x - tmp] = 0;
	  // 		  device->matrixBin[y][x + tmp] = 0;
	  // 		}
	  // 	      ++tmp;
	  // 	    }
	  // 	  ++x;
	  // 	}
	  //     ++y;
	  //   }
	  // y = 0;
	  // while (y < 480)
	  //   {
	  //     x = 0;
	  //     while (x < 640)
	  // 	{
	  // 	  if (device->matrixBin[y][x] == 1)
	  // 	    {
	  // 	      if (device->matrixBin[y][x - 1] == 1
	  // 		  && device->matrixBin[y + 1][x] == 1
	  // 		  && device->matrixBin[y][x - 1] == 1
	  // 		  && device->matrixBin[y + 1][x - 1] == 1)
	  // 		{
	  // 		  device->matrixBin[y][x] = 2;
	  // 		}
	  // 	    }
	  // 	  ++x;
	  // 	}
	  //     ++y;
	  //   }
	  x = 0;
	  y = 0;
	  SDL_FillRect(w, NULL, SDL_MapRGB(w->format, 0,0, 0));

	  while (y < 480)
	    {
	      x = 0;
	      while (x < 640)
		{
		  // if (device->matrixBin[y][x] == 1)
		  //   {
		  //     glColor3f(1.0, 0.0, 0.0);
		  //     glVertex3f(x, y, device->xyzCoordsCurr[y][x][2]);
		  //   }
		  // else if (device->matrixBin[y][x] == 2)
		  //   {
		  //     // std::cout << "Found corner "<<std::endl;
		  //     // glColor3f(1.0, 0.0, 1.0);
		  //     // glVertex3f(x, y, device->xyzCoordsCurr[y][x][2]);
		  //     drawPixel(x, device->xyzCoordsCurr[y][x][2], w, p, pos, 255, 255, 255);
		  //   }
		  // else
		  //   {
		  if (y < 160)
		    {
		      if (x < 213)
			{
			  glColor3f(1.0, 1.0, 1.0);
			  glVertex3f(x, y, device->xyzCoords[y][x][2]);
			}
		      else if (x < 213 * 2)
			{
			  glColor3f(0.0, 1.0, 1.0);
			  glVertex3f(x, y, device->xyzCoords[y][x][2]);
			}
		      else
			{
			  glColor3f(1.0, 0.0, 1.0);
			  glVertex3f(x, y, device->xyzCoords[y][x][2]);
			}
		    }
		  else if (y < 160 * 2)
		    {
		      if (x < 213)
			{
			  glColor3f(1.0, 0.0, 1.0);
			  glVertex3f(x, y, device->xyzCoords[y][x][2]);
			}
		      else if (x < 213 * 2)
			{
			  glColor3f(0.0, 0.0, 1.0);
			  glVertex3f(x, y, device->xyzCoords[y][x][2]);
			}
		      else
			{
			  glColor3f(1.0, 0.0, 0.0);
			  glVertex3f(x, y, device->xyzCoords[y][x][2]);
			}
		    }
		  else
		    {
		      if (x < 213)
			{
			  glColor3f(1.0, 1.0, 0.0);
			  glVertex3f(x, y, device->xyzCoords[y][x][2]);
			}
		      else if (x < 213 * 2)
			{
			  glColor3f(0.0, 1.0, 1.0);
			  glVertex3f(x, y, device->xyzCoords[y][x][2]);
			}
		      else
			{
			  glColor3f(0.0, 0.0, 1.0);
			  glVertex3f(x, y, device->xyzCoords[y][x][2]);
			}
		    }
		  glColor3f(1.0, 0.0, 0.0);
		  glVertex3f(x, y, device->xyzCoordsCurr[y][x][2]);
		  ++x;
		}
	      ++y;
	    }
	  glEnd();
	}
      SDL_Flip(w);
      App.Display();
      glFlush();
    }
  return 0;
}
