#ifndef ARDRONEPARSER_H
#define ARDRONEPARSER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <libxml++/libxml++.h>
#include <iostream>
#include "quadrotor_config.h"

class QuadrotorParser
{
    public:
        QuadrotorParser(int id);
        int readFile(std::string filepath,QuadrotorConfig *conf);
        void writeFile(std::string filepath,QuadrotorConfig *conf);
        virtual ~QuadrotorParser();
    protected:
    private:
	void parse(const xmlpp::Node* node,QuadrotorConfig *conf);
        int id;
};

#endif // ARDRONEPARSER_H

