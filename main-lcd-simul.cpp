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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>

#include "lcd_lib.h"
#include "font8x8.h"
#include "graph_class.hpp"

int main()
{
    lcd_init();                     // LCD initialization

    std::string text = "Vojta je bily clovek.";
    int len = text.length();
    for (int i = 0; i < len; i++)
    {
        Character letter({0 + i * 20, 0}, text.at(i), 2, {255, 255, 255}, {255, 255, 255});
        letter.draw();
    }

    //Line line({0, 0}, {400, 25}, {255, 255, 255}, {255, 255, 255});
    //line.draw();

    Circle circle({125, 125}, 37, {255, 255, 255}, {100, 100, 100});
    circle.draw();

    Rect rect({100, 100}, 50, 50, {255, 255, 255}, {100, 100, 100});
    rect.draw();
    

    cv::imshow( LCD_NAME, g_canvas );   // refresh content of "LCD"
    cv::waitKey( 0 );                   // wait for key 
}


