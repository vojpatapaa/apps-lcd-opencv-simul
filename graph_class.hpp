// **************************************************************************
//
//               Demo program for labs
//
// Subject:      Computer Architectures and Parallel systems
// Author:       Petr Olivka, petr.olivka@vsb.cz, 09/2019
// Organization: Department of Computer Science, FEECS,
//               VSB-Technical University of Ostrava, CZ
//
// File:         OpenCV simulator of LCD
//
// **************************************************************************

#include <cmath>
#include "lcd_lib.h"
#include "font8x8.h"

// Simple graphic interface

struct Point2D
{
    int32_t x, y;
};


struct RGB
{
    uint8_t r, g, b;
};


class GraphElement
{
	public:
		// foreground and background color
		RGB m_fg_color, m_bg_color;

		// constructor
		GraphElement( RGB t_fg_color, RGB t_bg_color ) :
			m_fg_color( t_fg_color ), m_bg_color( t_bg_color ) {}

		// ONLY ONE INTERFACE WITH LCD HARDWARE!!!
		void drawPixel( int32_t t_x, int32_t t_y )
		{
			lcd_put_pixel( t_x, t_y, convert_RGB888_to_RGB565( m_fg_color ) );
		}

		// Draw graphics element
		virtual void draw() = 0;

		// Hide graphics element
		virtual void hide() { swap_fg_bg_color(); draw(); swap_fg_bg_color(); }

	protected:
		// swap foreground and backgroud colors
		void swap_fg_bg_color() { RGB l_tmp = m_fg_color; m_fg_color = m_bg_color; m_bg_color = l_tmp; }

		// conversion of 24-bit RGB color into 16-bit color format
		uint16_t convert_RGB888_to_RGB565( RGB t_color )
		{
			return ((t_color.r >> 3) << 11) | ((t_color.g >> 2) << 5) | (t_color.b >> 3);
		}
};


class Pixel : public GraphElement
{

	private:
		// Position of Pixel
		Point2D m_pos;
	public:
		// constructor
		Pixel( Point2D t_pos, RGB t_fg_color, RGB t_bg_color ) : GraphElement( t_fg_color, t_bg_color ), m_pos( t_pos ) {}
		// Draw method implementation
		virtual void draw()
		{
			drawPixel( m_pos.x, m_pos.y );
		}
};


class Circle : public GraphElement
{
	private:
		// Center of circle
		Point2D m_center;
		// Radius of circle
		int32_t m_radius;
	public:

		Circle( Point2D t_center, int32_t t_radius, RGB t_fg, RGB t_bg ) :
			GraphElement( t_fg, t_bg ), m_center( t_center ), m_radius( t_radius ) {}

		void DrawCirclePoint(int x, int y)
		{
			drawPixel(m_center.x+x, m_center.y+y);
			drawPixel(m_center.x-x, m_center.y+y);
			drawPixel(m_center.x+x, m_center.y-y);
			drawPixel(m_center.x-x, m_center.y-y);
			drawPixel(m_center.x+y, m_center.y+x);
			drawPixel(m_center.x-y, m_center.y+x);
			drawPixel(m_center.x+y, m_center.y-x);
			drawPixel(m_center.x-y, m_center.y-x);
		}

		void draw()
		{

			this->swap_fg_bg_color();

			for(int y = this->m_center.y - this->m_radius; y < this->m_center.y + this->m_radius; y++)
			{
				for(int x = this->m_center.x - this->m_radius; x < this->m_center.x + this->m_radius; x++)
				{
					if(std::sqrt(std::pow(x - m_center.x, 2) + std::pow(y - m_center.y, 2)) < m_radius)
					{
						this->drawPixel(x, y);
					}
				}
			}

			this->swap_fg_bg_color();

			int r = m_radius;
			int x = 0;
			int y = m_radius;
			int d = 3 - 2 * r;
			DrawCirclePoint(x, y);
			while (y >= x)
			{
				if (d > 0)
				{
					y--;
					d = d + 4 * (x - y) + 10;
				}
				else
				{
					d = d + 4 * x + 6;
				}

				x++;

				DrawCirclePoint(x, y);

			}

		}
};


class Character : public GraphElement
{
	private:
		// position of character
		Point2D m_pos;
		// character
		char m_character;

	public:

		Character( Point2D t_pos, char t_char, RGB t_fg, RGB t_bg ) :
		  GraphElement( t_fg, t_bg ), m_pos( t_pos ), m_character( t_char ) {};

		void draw()
		{
			for(int i = 0; i < 8; i++)
			{
				for(int j = 0; j < 8; j++)
				{
					uint8_t character = m_character;
					int mask = 1;
					if(((g_font8x8[character][i] >> j) & mask) == 1)
					{
						this->drawPixel(m_pos.x + j, m_pos.y + i);
					}
				}
			}
		};
};


class Line : public GraphElement
{
	private:
		// the first and the last point of line
		Point2D m_pos1, m_pos2;

	public:

		Line( Point2D t_pos1, Point2D t_pos2, RGB t_fg, RGB t_bg ) :
		  GraphElement( t_fg, t_bg ), m_pos1( t_pos1 ), m_pos2( t_pos2 ) {}

	void draw()
	{
		int x1 = m_pos1.x;
		int y1 = m_pos1.y;
		int x2 = m_pos2.x;
		int y2 = m_pos2.y;

		int dx = std::abs(x2 - x1);
		int dy = std::abs(y2 - y1);

		int sx = (x1 < x2) ? 1 : -1;
		int sy = (y1 < y2) ? 1 : -1;

		int err = dx - dy;

		while(true)
		{
			drawPixel(x1, y1);

			if (x1 == x2 && y1 == y2)
			{
				break;
			}

			int e2 = 2 * err;

			if (e2 > -dy)
			{
				err -= dy;
				x1 += sx;
			}

			if (e2 < dx)
			{
				err += dx;
				y1 += sy;
			}
		}
	}
};

class Rect : GraphElement
{
private:
	Point2D m_pos;
	u_int16_t m_width;
	u_int16_t m_height;
public:
	Rect(Point2D t_pos, uint16_t t_width, uint16_t t_height, RGB t_fg, RGB t_bg ) :
		GraphElement( t_fg, t_bg ), m_pos( t_pos ), m_width( t_width ), m_height( t_height ) {}

	void draw() 
	{
		for (int y = m_pos.y; y < m_pos.y + m_height; y++)
		{
			Line line({m_pos.x, y}, {m_pos.x + m_width, y}, this->m_bg_color, this->m_fg_color);
			line.draw();
		}

		Line line1({m_pos.x, m_pos.y}, {m_pos.x + m_width, m_pos.y}, this->m_fg_color, this->m_bg_color);
		line1.draw();
		
		Line line2({m_pos.x + m_width, m_pos.y}, {m_pos.x + m_width, m_pos.y + m_height}, this->m_fg_color, this->m_bg_color);
		line2.draw();

		Line line3({m_pos.x + m_width, m_pos.y + m_height}, {m_pos.x, m_pos.y + m_height}, this->m_fg_color, this->m_bg_color);
		line3.draw();

		Line line4({m_pos.x, m_pos.y + m_height}, {m_pos.x , m_pos.y}, this->m_fg_color, this->m_bg_color);
		line4.draw();
	}
};




