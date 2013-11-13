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
 *  Authors : Borja Menéndez Moreno <b.menendez.moreno@gmail.com>
 *            José María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <iostream>

/*************************************************************
 * FOR AUTOMATAS
 *************************************************************/
const std::string TIME_DEFAULT = "100";

/*************************************************************
 * FOR NODES
 *************************************************************/
const int RADIUS_NORMAL = 20;
const int RADIUS_INIT = 15;
const int Y_NODE_IDEAL = 35;

const std::string ITEM_NAME_NODE = "state";

/*************************************************************
 * FOR TRANSITIONS
 *************************************************************/
const int LINE_WIDTH = 1;
const int ARROW_LENGTH = 10;
const int ARROW_TIP_LENGTH = 10;
const int ARROW_WIDTH = 10;
const bool END_ARROW = true;

const int SQUARE_SIDE = 10;
const int SQUARE_SIDE_MID = SQUARE_SIDE / 2;
const int Y_TRANS_IDEAL = 25;

const std::string ITEM_NAME_TRANSITION = "transition";

/*************************************************************
 * FOR ITEMS IN GENERAL
 *************************************************************/
const std::string ITEM_COLOR_BLACK = "black";
const std::string ITEM_COLOR_BLUE = "blue";
const std::string ITEM_COLOR_GREEN = "green";
const std::string ITEM_COLOR_GREY = "grey";
const std::string ITEM_COLOR_PURPLE = "purple";
const std::string ITEM_COLOR_RED = "red";
const std::string ITEM_COLOR_WHITE = "white";
const std::string ITEM_COLOR_YELLOW = "yellow";

const int LETTER_WIDTH = 2;
const float PIXELS_PER_LETTER = 3.5;

/*************************************************************
 * FOR STD OUTPUT COLOR (DEBUG)
 *************************************************************/
const bool DEBUG = true;

const std::string BEGIN_RED = "\033[1;31m";
const std::string BEGIN_GREEN = "\033[1;32m";
const std::string BEGIN_YELLOW = "\033[1;33m";
const std::string BEGIN_BLUE = "\033[1;34m";
const std::string BEGIN_PURPLE = "\033[1;35m";
const std::string BEGIN_LBLUE = "\033[1;36m";
const std::string BEGIN_GREY = "\033[1;37m";
const std::string END_COLOR = "\033[0m";

const std::string VISUAL = "[[[[ ";
const std::string GUISUB = "[[[ ";
const std::string GUIELEM = "[[ ";
const std::string OTHER = "[ ";

#endif