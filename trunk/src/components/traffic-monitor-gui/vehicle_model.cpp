#include <stdio.h>
#include <string.h>
#include "vehicle_model.h"

const char* VehicleModel::models_names[MAX_MODELS] = {
   "Invalid",
   "Motocycle",
   "Car",
   "SUV",
   "Van",
   "Truck",
};

const char* VehicleModel::models_description[MAX_MODELS] = {
   "INV",
   "Motorcycle",
   "Car",
   "SUV",
   "Van",
   "Truck/Bus",
};

/**
 *  The units are in meteres
 *
 *   z / 
 *    /  
 *   |  / 
 * x | / 
 *   |/____ 
 *      y 
 *
 */
T3dmodel VehicleModel::models_dimensions[MAX_MODELS] = {
  
   /** invalid vehicle*/
   {0,0,0},
   
   /** MOTO CYCLE*/
   {1.2, 0.5, 2},
   
   /** CAR **/
   {1.3, 1.6, 4},

   /** SUV **/
   {1.8, 1.7, 4.5},
   
   /** VAN **/
   {2, 2, 7},
   
   /** TRUCK **/
   {3.5, 2.5, 13.5},
};

/**
 *
 */
const char* VehicleModel::get_model_name(tvehicle_category category){
   
   if (VALID_CATEGORY(category))
      return models_names[category];
   else
      return models_names[INVALID_VEHICLE_CLASS];
}

/**
 *
 */
float VehicleModel::get_category_dimension_x(tvehicle_category category){
   
   if (VALID_CATEGORY(category))
      return models_dimensions[category].x;
   else
      return -1;
}

/**
 *
 */
float VehicleModel::get_category_dimension_y(tvehicle_category category){
   
   if (VALID_CATEGORY(category))
      return models_dimensions[category].y;
   else
      return -1;
}

/**
 *
 */
float VehicleModel::get_category_dimension_z(tvehicle_category category){
   
   if (VALID_CATEGORY(category))
      return models_dimensions[category].z;
   else
      return -1;
}

/**
 *
 */
const char* VehicleModel::get_model_desc(tvehicle_category category){
   
   if (VALID_CATEGORY(category))
      return models_description[category];
   else
   {
      return models_description[INVALID_VEHICLE_CLASS];
   }
}

/**
 *
 */
tvehicle_category VehicleModel::get_model_id(const std::string model_name){

   tvehicle_category i;
  
   for (i=MOTORCYCLE; i<MAX_MODELS; i++)
   {
      if (strcmp(model_name.c_str(), models_names[i]) == 0)
      {
         printf(" %s has been selected\n",models_names[i]);
         return i;
      }
   }
  
   return INVALID_VEHICLE_CLASS;
}
