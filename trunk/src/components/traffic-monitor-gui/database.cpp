#include <iostream>
#include "database.h"
#include <stdlib.h>

using namespace std;
using namespace mysqlpp;

namespace trafficmonitor{

DataBase database;

/**
 *
 */
DataBase::DataBase()
{
   
   m_conn = new Connection(false);
}

/**
 *
 */
bool DataBase::connect()
{
   try
   {
      if(m_conn->connect("trafficmonitor_db", "localhost", "trafficmonitor", "trafficmonitor123"))
      {
         cout << "Database connected successfully" << endl;
      }
      else
      {
         cerr << "Error: Can't connect to the database" << endl;
      }
   }
   catch (const std::exception& e)
   {
      cerr << "Error while connecting to the database. Details: " << e.what() << endl;
      exit(1);
   }
}

/**
 *
 */
bool DataBase::disconnect()
{

}

/**
 *
 */
// bool DataBase::store (const Vehicle& vehicle)
// {
//    std::stringstream sql_insert;
   
//    Query query = m_conn->query();
//    query << "INSERT INTO traffic_stats VALUES("
//          << vehicle.get_id()
//          << ",'"
//          << VehicleModel::get_model_desc(vehicle.get_matched_class())
//          << "',"
//          << vehicle.get_speed()
//          << ");";

//    const std::string& ins_query = sql_insert.str();
   
//    try
//    {
//       query.execute();
//    }
//    catch (const std::exception &e)
//    {
      
//    }
// }

/**
 *
 */
mysqlpp::StoreQueryResult DataBase::getLastVehicles(int limit_query)
{
   /* Now SELECT */
   Query query = m_conn->query();
   query << "select * from traffic_stats order by timestamp desc limit " << limit_query;
   StoreQueryResult ares = query.store();   
   return ares;
}

/**
 *
 */
int DataBase::getCount(std::string category)
{
   /* Now SELECT */
   Query query = m_conn->query();
   query << "select category, count(*) from traffic_stats where category = \"" << category << "\"";
   StoreQueryResult result = query.store();

   // This query returns a table in the following format, so we just pick the count(*) column value
   //
   // +----------+----------+
   // | category | count(*) |
   // +----------+----------+
   // | Van      |        7 |
   // +----------+----------+
   //

   return stoi(result[0]["count(*)"].c_str());
}


} // trafficmonitor namespace
