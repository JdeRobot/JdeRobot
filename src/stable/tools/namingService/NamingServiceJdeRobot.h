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

#include <jderobot/namingService.h>
#include <log/Logger.h>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <vector>
#include <string>

namespace NamingService
{

	/**
	 * NamingServiceJdeRobot class implements a simple and efficient naming service
	 * for components. Allow (de)serializer of the naming table.
	 */

	class NamingServiceJdeRobot: virtual public jderobot::NamingService
	{

	public:

		/**
		* \brief Constructor
		*
		* @param propertyPrefix Prefix to get specific config of ICE file
		 */
		NamingServiceJdeRobot(std::string& propertyPrefix);

		/**
		 * \brief Bind the new node in the table. If already exists a node with the same name,
		 * it's replace by the new.
		 *
		 * @param node The node to bind
		 * @param current the Ice data
		 *
		 */
		virtual void bind (const jderobot::NamingNodePtr& node, const Ice::Current& current);

		/**
		 * \brief Unbind a node of the table.
		 *
		 * @param node The node to unbind
		 * @param current the Ice data
		 *
		 */
		virtual void unbind (const jderobot::NamingNodePtr& node, const Ice::Current& current);

		/**
		 * \brief Resolve a petition given a name of component
		 *
		 * @param name The name of component to search
		 * @param current the Ice data
		 *
		 */
		virtual jderobot::NodeContainerPtr resolveByName (const std::string& name, const Ice::Current& current);

		/**
		 * \brief Resolve a petition given an interface
		 *
		 * @param name The name of interface to search
		 * @param current the Ice data
		 *
		 */
		virtual jderobot::NodeContainerPtr resolveByInterface (const std::string& interface, const Ice::Current& current);

	private:

		bool remove (const jderobot::NamingNodePtr& node);
		bool exists (const jderobot::NamingNodePtr& node);

		bool save();
		bool load();
		std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);



		jderobot::NodeContainerPtr mNodes;
	};

}
