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

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <string>

#include "lcd_lib.h"
#include "font8x8.h"
#include "graph_class.hpp"

Point2D center({480/2, 320/2});

double secondsAngle = -0.5 * M_PI;
double minutesAngle = -0.5 * M_PI;
double hoursAngle = -0.5 * M_PI;

Line secondsPointer(center, {0, 30}, {255, 255, 255}, {0, 0, 0});
Line minutesPointer(center, {0, 20}, {255, 255, 255}, {0, 0, 0});
Line hoursPointer(center, {0, 10}, {255, 255, 255}, {0, 0, 0});

int main()
{
    // LCD initialization
    lcd_init();  

    Circle circle(center, 120, {255, 255, 255}, {0, 0, 0});
    circle.draw();

    double lineAngle = -0.5*M_PI;
    for (int i = 0; i < 60; i++)
    {
        Line line({center.x + std::cos(lineAngle) * 115.0, center.y + std::sin(lineAngle) * 115.0}, {center.x + std::cos(lineAngle) * 120.0, center.y + std::sin(lineAngle) * 120.0}, {255, 255, 255}, {0, 0, 0});
        line.draw();

        lineAngle += 2*M_PI / 60.0;
    }


    double numberAngle = -0.5*M_PI + (2*M_PI / 12.0);
    for (int i = 0; i < 12; i++)
    {
        std::string numberStr = std::to_string(i + 1);
        if(numberStr.length() == 1)
        {
            numberStr.insert(0, 1, '0');
        }
        int len = numberStr.length();
        for (int j = 0; j < len; j++)
        {
            Character letter({center.x + std::cos(numberAngle) * 95 + (j * 8) - 4, center.y + std::sin(numberAngle) * 95 - 4}, numberStr.at(j), {255, 255, 255}, {0, 0, 0});
            letter.draw();
        }
        

        numberAngle += 2*M_PI / 12.0;
    }
    
    
    
    

    while (1)
    {
        secondsPointer.hide();
        minutesPointer.hide();
        hoursPointer.hide();

        secondsAngle += 2*M_PI/60.0;
        minutesAngle += 2*M_PI/(60.0 * 60.0);
        hoursAngle += 2*M_PI/(60.0 * 60.0 * 24.0);

        secondsPointer.m_pos2 = {center.x + std::cos(secondsAngle) * 80.0, center.y + std::sin(secondsAngle) * 80.0};
        minutesPointer.m_pos2 = {center.x + std::cos(minutesAngle) * 60.0, center.y + std::sin(minutesAngle) * 60.0};
        hoursPointer.m_pos2 = {center.x + std::cos(hoursAngle) * 40.0, center.y + std::sin(hoursAngle) * 40.0};

        printf("%f, %f, %f\n", secondsAngle, minutesAngle, hoursAngle);

        secondsPointer.draw();
        minutesPointer.draw();
        hoursPointer.draw();


        cv::imshow( LCD_NAME, g_canvas );   // refresh content of "LCD"
        cv::waitKey( 0 );                   // wait for key 
        sleep(1);
    }
    

    cv::imshow( LCD_NAME, g_canvas );   // refresh content of "LCD"
    cv::waitKey( 0 );                   // wait for key 
}


