/*
 *  Copyright (C) 1997-2013 JDERobot Developers Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors : Borja Menéndez <borjamonserrano@gmail.com>
 *
 */

#include "sensors.h"
#include <visionlib/colorspaces/colorspacesmm.h>
/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
Sensors::Sensors ( Ice::CommunicatorPtr ic ) {
    this->ic = ic;
    Ice::PropertiesPtr prop = this->ic->getProperties();
    
    this->filter = prop->getPropertyAsInt("FollowBall.filter");
    
    Ice::ObjectPrx base = ic->propertyToProxy("FollowBall.Camera.Proxy");
    if (base == 0)
        throw "Could not create proxy with Camera";
    
    cameraprx = jderobot::CameraPrx::checkedCast(base);
    if (cameraprx == 0)
        throw "Invalid Camera proxy";
    
    jderobot::ImageDataPtr data = cameraprx->getImageData(colorspaces::ImageRGB8::FORMAT_RGB8.get()->name);
    this->image.create(data->description->height, data->description->width, CV_8UC3);
    
    this->updateValues();    
    gettimeofday(&this->a, NULL);
    
    pthread_mutex_init(&mutex, NULL);
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
Sensors::~Sensors () {
    if (this->ic)
        this->ic->destroy();
}

/*************************************************************
 * GETTERS
 *************************************************************/
cv::Mat& Sensors::getImage () {
    return this->image;
}

float Sensors::getErrorX () {
    return this->errorX;
}

float Sensors::getErrorY () {
    return this->errorY;
}

int Sensors::getRH () {
    return this->valueRH;
}

int Sensors::getGS () {
    return this->valueGS;
}

int Sensors::getBV () {
    return this->valueBV;
}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
void Sensors::update () {    
    jderobot::ImageDataPtr data = cameraprx->getImageData(colorspaces::ImageRGB8::FORMAT_RGB8.get()->name);
    pthread_mutex_lock(&this->mutex);
    
    memcpy((unsigned char *) this->image.data, &(data->pixelData[0]), this->image.cols * this->image.rows * 3);
    
    struct timeval b;
    gettimeofday(&b, NULL);
    long totala = this->a.tv_sec * 1000000 + this->a.tv_usec;
    long totalb = b.tv_sec * 1000000 + b.tv_usec;
    long diff = (totalb - totala) / 1000;
    
    if (diff > 10000) {
        this->updateValues();
        gettimeofday(&this->a, NULL);
    }
    
    switch (this->filter) {
    case 0: // RGB
        break;
    case 1: // HSV
        cv::cvtColor(this->image, this->image, CV_RGB2HSV);
        break;
    default: // HSV by default
        cv::cvtColor(this->image, this->image, CV_RGB2HSV);
        break;
    }
    
    cv::Mat imageBackGroundGRAY;
    imageBackGroundGRAY.create(data->description->height, data->description->width, CV_8UC1);

    for ( int x = 0; x < this->image.cols; x++ ) {
        for ( int y = 0; y < this->image.rows; y++ ) {
            int indice = y * this->image.step + x * this->image.channels();
            int indiceGray = y * imageBackGroundGRAY.step + x * imageBackGroundGRAY.channels();
                 
            int imageh = (int) this->image.data[indice];
            int images = (int) this->image.data[indice+1];
            int imagev = (int) this->image.data[indice+2];
            
            if ( (imageh >= this->minValueRH) && (imageh <= this->maxValueRH)
                && (images >= this->minValueGS) && (images <= this->maxValueGS)
                && (imagev >= this->minValueBV) && (imagev <= this->maxValueBV) ) {
                imageBackGroundGRAY.data[indiceGray] = 255;
            } else {
                imageBackGroundGRAY.data[indiceGray] = 0;
            }
        }
    }

    cv::Mat threshold_output;
    int thresh = 50;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::threshold(imageBackGroundGRAY, threshold_output, thresh, 255, cv::THRESH_BINARY);
    cv::findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    /// Approximate contours to polygons + get bounding rects and circles
    std::vector<std::vector<cv::Point> > contours_poly(contours.size());
    std::vector<cv::Rect> boundRect;
    std::vector<cv::Point2f>center(1);
    std::vector<float>radius(1);

    cv::Mat contoursAux;
    for ( unsigned i = 0; i < contours.size(); i++ ) {
        contoursAux = cv::Mat(contours[i]);
        cv::approxPolyDP(contoursAux, contours_poly[i], 3, true);
    }

    int maximo = 0;
    int indice_maximo = 0;
    for ( unsigned i= 0; i < contours_poly.size(); i++ ) {
        contoursAux = cv::Mat(contours_poly[i]);
        double area0 = cv::contourArea(contoursAux);
        if (area0 > maximo) {
            maximo = area0;
            indice_maximo = i;
        }
    }

    if (contours_poly.size() > 0){
        contoursAux = cv::Mat(contours_poly[indice_maximo]);
        boundRect.push_back(boundingRect(cv::Mat(contours_poly[indice_maximo])));
        cv::minEnclosingCircle( (cv::Mat)contours_poly[indice_maximo], center[0], radius[0] );
    }
    
    /// Draw polygonal contour + bonding rects + circles
    for ( unsigned i = 0; i < boundRect.size(); i++ ) {
        cv::Scalar color = cv::Scalar(255, 0, 0);
        cv::circle(this->image, center[i], (int)radius[i], color, 2, 8, 0);
    }

    if (boundRect.size() > 0) {
        this->errorX = 320/2 - center[0].x;
        this->errorY = 240/2 - center[0].y;
    } else {
        this->errorX = 0.0;
        this->errorY = 0.0;
    }

    #ifndef COMPILEFORNAO
    cv::line(this->image, cv::Point(320/2, 0), cv::Point(320/2, 240), cv::Scalar(0, 0, 255), 1);
    cv::line(this->image, cv::Point(0, 240/2), cv::Point(320, 240/2), cv::Scalar(0, 0, 255), 1);
    #endif

    pthread_mutex_unlock(&this->mutex);
    
    imageBackGroundGRAY.release();
    threshold_output.release();
    contoursAux.release();
    
    contours.clear();
    hierarchy.clear();
    contours_poly.clear();
    boundRect.clear();
    center.clear();
    radius.clear();
}

void Sensors::updateValues () {
    std::ifstream infile("filter.txt");
    std::string line;
    int i = 0;
    
    while ( std::getline(infile, line) ) {
        std::istringstream iss(line);
        int n;
        iss >> n;
        
        switch (i) {
        case 0: // R/H
            this->valueRH = n;
            break;
        case 1: // G/S
            this->valueGS = n;
            break;
        case 2: // B/V
            this->valueBV = n;
            break;
        }
        i++;
    }
    
    int restRH, restGS, restBV;
    
    if (this->valueRH + 25 > 255) {
        this->maxValueRH = 255;
        restRH = this->valueRH + 25 - 255;
    } else {
        this->maxValueRH = this->valueRH + 25;
        restRH = 0;
    }
    if (this->valueRH - 25 < 0) {
        this->minValueRH = 0;
        restRH = -(this->valueRH - 25);
        this->maxValueRH += restRH;
    } else {
        this->minValueRH = this->valueRH - 25;
        if (restRH != 0)
            this->minValueRH -= restRH;
    }
    
    if (this->valueGS + 25 > 255) {
        this->maxValueGS = 255;
        restGS = this->valueGS + 25 - 255;
    } else {
        this->maxValueGS = this->valueGS + 25;
        restGS = 0;
    }
    if (this->valueGS - 25 < 0) {
        this->minValueGS = 0;
        restGS = -(this->valueGS - 25);
        this->maxValueGS += restGS;
    } else {
        this->minValueGS = this->valueGS - 25;
        if (restGS != 0)
            this->minValueGS -= restGS;
    }
    
    if (this->valueBV + 25 > 255) {
        this->maxValueBV = 255;
        restBV = this->valueBV + 25 - 255;
    } else {
        this->maxValueBV = this->valueBV + 25;
        restBV = 0;
    }
    if (this->valueBV - 25 < 0) {
        this->minValueBV = 0;
        restBV = -(this->valueBV - 25);
        this->maxValueBV += restBV;
    } else {
        this->minValueBV = this->valueBV - 25;
        if (restBV != 0)
            this->minValueBV -= restBV;
    }
}

void Sensors::RGBtoHSV ( double r, double g, double b, double *h, double *s, double *v ) {
	double max = (r > g && r > b)? r : (g > b)? g : b;
	double min = (r < g && r < b)? r : (g < b)? g : b;
	
	*h = 0;
	*s = 0;
	*v = max;
	
	if (*v != 0)
	    *s = (*v - min) / *v;
	else
	    *s = 0;

	if (*v == r)
	    *h = 60 * (g - b) / (*v - min);
	if (*v == g)
	    *h = 120 + 60 * (b - r) / (*v - min);
    if (*v == b)
        *h = 240 + 60 * (r - g) / (*v - min);
        
    if (*h < 0)
        *h += 360;
    
//    *v *= 255;
    *s *= 255;
    *h /= 2;
}
