#include "sensors.h"

Sensors::Sensors(jderobot::ImageDataPtr data) {
    image.create(data->description->height, data->description->width, CV_8UC3);
    
    this->panSpeed = 0.0;
    this->tiltSpeed = 0.0;
}

void Sensors::update(jderobot::ImageDataPtr data)
{
    memcpy((unsigned char *) image.data ,&(data->pixelData[0]), image.cols*image.rows * 3);

    cv::Mat gray;
    cv::Mat resta;
    cv::Mat imageBackGround;

    imageBackGround = image.clone();

    for(int x = 0; x < imageBackGround.cols; x++){
        for(int y = 0; y < imageBackGround.rows; y++){

            int indice = y*imageBackGround.step + x * imageBackGround.channels();

            if((imageBackGround.data[indice+2] < 157)
                && (imageBackGround.data[indice] > 250)){
          //          && imageBackGround.data[indice+1] > 60 && imageBackGround.data[indice+1] < 133
          //          && imageBackGround.data[indice+2] < 50){
                imageBackGround.data[indice] = 255;
                imageBackGround.data[indice+1] = 255;
                imageBackGround.data[indice+2] = 255;

            }else{
                imageBackGround.data[indice] = 0;
                imageBackGround.data[indice+1] = 0;
                imageBackGround.data[indice+2] = 0;
            }

        }
    }

//    cv::imread("/home/ahcorde/tfm/trunk/visualizadoresImagenes/followBall/DATA/01.jpg");

//    imageBackGround = cv::Scalar(0,0,0);

    resta = imageBackGround.clone();

    cv::cvtColor(resta, gray, CV_RGB2GRAY);

    cv::Mat threshold_output;
    int thresh = 50;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::threshold( gray, threshold_output, thresh, 255, cv::THRESH_BINARY );
    cv::findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

    /// Approximate contours to polygons + get bounding rects and circles
    std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
    std::vector<cv::Rect> boundRect;
    std::vector<cv::Point2f>center( 1 );
    std::vector<float>radius( 1 );

    for( unsigned i = 0; i < contours.size(); i++ ){
        cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
    }

    int maximo = 0;
    int indice_maximo = 0;
    for(unsigned i= 0; i < contours_poly.size(); i++ ){
        double area0 = cv::contourArea(contours_poly[i]);
        if(area0>maximo){
            maximo = area0;
            indice_maximo = i;
        }
    }

    if(contours_poly.size()>0){
        boundRect.push_back(boundingRect( cv::Mat(contours_poly[indice_maximo]) ) );
        cv::minEnclosingCircle( (cv::Mat)contours_poly[indice_maximo], center[0], radius[0] );
    }

    /// Draw polygonal contour + bonding rects + circles
    for( unsigned i = 0; i< boundRect.size(); i++ ){
           cv::Scalar color = cv::Scalar( 255, 0, 0 );
           //cv::drawContours( drawing, contours_poly, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );
           cv::rectangle( image, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
           cv::circle( image, center[i], (int)radius[i], color, 2, 8, 0 );
    }

    if(boundRect.size()>0){
        float error_X = 160/2 - center[0].x;
        float error_Y = 120/2 - center[0].y;

        std::cout << "error_X: "<< error_X/(160/2) << " error_Y: " << error_Y/(120/2) << std::endl;

        this->panSpeed = -error_X/(160/2);
        this->tiltSpeed = -error_Y/(120/2);
    }

    cv::line(image, cv::Point(160/2, 0), cv::Point(160/2, 120), cv::Scalar(0, 0, 255), 1);
    cv::line(image, cv::Point(0, 120/2), cv::Point(160, 120/2), cv::Scalar(0, 0, 255), 1);


//    resta.copyTo(image);

}

cv::Mat Sensors::getImage() {
    cv::Mat result = image.clone();

    return result;
}

float Sensors::getPanSpeed () {
    return this->panSpeed;
}

float Sensors::getTiltSpeed () {
    return this->tiltSpeed;
}
