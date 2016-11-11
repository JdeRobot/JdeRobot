/*
 *  Copyright (C) 2014 JdeRobot developers
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
 *  Authors : Roberto Calvo Palomino <rocapal [at] gsyc [dot] urjc [dot] es>
 *
 */

#ifndef NS_H
#define NS_H


#include <namingService.h>
#include <logger/Logger.h>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

namespace jderobot
{

	class ns;
	typedef boost::shared_ptr<ns> nsPtr;


	/**
	 * ns class implements facade to access namingService ICE
	 */

	class ns
	{
	public:

		/**
		 * \brief Constructor
		 *
		 * @param ic The communicator ICE
		 * @param proxy The proxy to connect with naming service
		 */
		ns(Ice::CommunicatorPtr& ic, std::string proxy);


		ns(const Ice::CommunicatorPtr& ic, const std::string& configKey, bool active=true);

		~ns();

		/**
		 * \brief Send bind petition to naming service
		 *
		 * @param name The name of the component
		 * @param EndPoint The endpoint well formed (default -h 0.0.0.0 -p 9999)
		 * @param interface The name of interface well formed (use ice_staticId() of your ObjectPtr)
		 */
		void bind (std::string name, std::string Endpoint, std::string interface );


		/**
		 * \brief Send Unbind petition to naming service
		 *
		 * @param name The name of component
		 *
		 */
		void unbind (std::string name);

		/**
		 * \brief Send unbind petition to every component register
		 *
		 */
		void unbindAll();


		/**
		 * \brief Send a resolve petition given a name of component
		 *
		 * @param name The name of component to search
		 */
		jderobot::NodeContainerPtr resolveByName(std::string name);

		/**
		 * \brief Send a resolve petition given a name of interface
		 *
		 * @param name The name of component to search
		 */
		jderobot::NodeContainerPtr resolveByInterface(std::string name);

		/**
		 * \brief Translate a naming node to proxy string
		 *
		 * @param node The naming node well formed
		 *
		 * @return string The string that represents the connection (example: cameraA:tcp -h 127.0.0.1 -p 9999)
		 */
		std::string getProxyStr (const NamingNode& node);


		/**
		 * \brief Return a NamingNodePtr using the registered name
		 */
		jderobot::NamingNodePtr getProxy(std::string name);


		/**
		 * \brief Returns the ICE proxy to namingService
		 */
		jderobot::NamingServicePrx getIceProxy();


		/**
 * \brief Return a NamingNodePtr using the registered name
 */
		std::string getProxyStrFromName(std::string name);

	private:

		NamingServicePrx mNamingService;

		std::vector<std::string> mBinds;

		std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);

		bool constructor(const Ice::CommunicatorPtr& ic, const std::string& proxy);

		};

}

#endif
