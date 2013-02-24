/*
 *  Copyright (C) 1997-2008 JDE Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *
 *  Authors : Redouane Kachach <redo.robot at gmail.com>
 *
 */
#ifndef _VEHICLE_MODEL_
#define _VEHICLE_MODEL_

#include <string>

/**
 *
 */
typedef enum models_ids 
{
   INVALID_VEHICLE_CLASS,
   MOTORCYCLE,
   CAR,
   SUV,
   VAN,
   TRUCK,
   MAX_MODELS
} tvehicle_category;

/**
 *
 */
typedef struct 
{
   float x;
   float y;
   float z;
} T3dmodel;

#define VALID_CATEGORY(c) (c>INVALID_VEHICLE_CLASS && c<MAX_MODELS)

/**
 *
 */
inline tvehicle_category& operator++(tvehicle_category& m) 
{ 
   return (m=tvehicle_category(m+1)); 
} 

/**
 *
 */
inline tvehicle_category operator++(tvehicle_category& m,int) 
{ 
   tvehicle_category mret = m; 
   ++m; 
   return mret; 
} 

/**
 *
 */
class VehicleModel{

public:
   
   /**
    *
    */
   static const char* get_model_name(tvehicle_category category);

   /**
    *
    */
   static const char* get_model_desc(tvehicle_category category);

   /**
    * Returns the category dimension (x,y or z) if the category is valid
    * and -1 if the passed category is not valid.
    */
   static float get_category_dimension_x(tvehicle_category category);
   static float get_category_dimension_y(tvehicle_category category);
   static float get_category_dimension_z(tvehicle_category category);
   
   /**
    *
    */
   static tvehicle_category get_model_id(const std::string model_name);

private:
   
   static const char* models_names[MAX_MODELS];
   static const char* models_description[MAX_MODELS];
   static T3dmodel models_dimensions[MAX_MODELS];
};

#endif
