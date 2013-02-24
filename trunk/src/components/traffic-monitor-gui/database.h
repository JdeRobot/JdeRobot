#ifndef DATABASE_H
#define DATABASE_H

#include <mysql++.h>

namespace trafficmonitor{

class DataBase
{
public:
   
   /**
    *
    */
   DataBase();
   virtual ~DataBase(){};

   /**
    *
    */
   bool connect();

   /**
    *
    */
   bool disconnect();

   /**
    *
    */
   int getCount(std::string category);

   /**
    *
    */ 
   mysqlpp::StoreQueryResult getLastVehicles(int limit_query);

private:

   mysqlpp::Connection *m_conn; 
};

extern DataBase database;

}

#endif
