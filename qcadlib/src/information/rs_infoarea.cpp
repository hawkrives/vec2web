/****************************************************************************
** $Id: rs_infoarea.cpp,v 1.2 2004/07/09 23:54:59 andrew Exp $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "rs_infoarea.h"


#include "rs_infoarea.h"
#include "rs_math.h"
#include "rs_debug.h"


/**
 * Constructor.
 */
RS_InfoArea::RS_InfoArea() {
}



/**
 * Destructor.
 */
RS_InfoArea::~RS_InfoArea() {}



/** 
 * Adds a point to the internal list
 *
 * @param p co-ordinate of the point
 */
void RS_InfoArea::addPoint(const RS_Vector& p) {
	if (thePoints.empty()) {
		baseY = p.y;
	}
	
	thePoints.append(p);
}



/**
 * Resets the points.
 */
void RS_InfoArea::reset() {
	thePoints.clear();
	area = 0.0;
	circumference = 0.0;
}



/**
 * Closes the polygon if it is not closed already.
 */
void RS_InfoArea::close() {
	if (isValid() && isClosed()==false) {
		
		thePoints.append(thePoints.first());

		RS_DEBUG->print("RS_InfoArea::close: closed");
	}
}



/**
 * @retval true If the area is closed (i.e. start point and end point are
 *   identical)
 * @retval false Otherwise.
 */
bool RS_InfoArea::isClosed() {
	return (thePoints.first().distanceTo(thePoints.last())<1.0e-4);
}



/**
 * @retval true If the area is defined (i.e. there are at least 3 points)
 * @retval false If there are only two or less points.
 */
bool RS_InfoArea::isValid() {
	RS_DEBUG->print("RS_InfoArea::isValid: count: %d", thePoints.count());
	return (thePoints.count()>2);
}



/**
 * Calculates the area and the circumference of the area.
 */
void RS_InfoArea::calculate() {
	area = 0.0;
	circumference = 0.0;
	RS_Vector ptFirst; 
	RS_Vector p1;
	RS_Vector p2;
	
	// at least 3 points needed for an area
	if (isValid()) {
		ptFirst = thePoints.last();
		thePoints.pop_back();
		
		p1 = ptFirst;
		while (!thePoints.empty()) {
			p2 = thePoints.last();
			thePoints.pop_back();
			
			area += calcSubArea(p1, p2);
			circumference += p1.distanceTo(p2);
			//if (p1 != ptFirst) {
			//	delete p1;
			//}
			p1 = p2;
		}
		area += calcSubArea(p1, ptFirst);
		circumference += p1.distanceTo(ptFirst);
		//delete p1;
		//delete ptFirst;
	}

	//thePoints.clear();
	area = fabs(area);
}



/**
 * Calculates a sub area.
 * 
 * @param p1 first point
 * @param p2 second point
 */
double RS_InfoArea::calcSubArea(const RS_Vector& p1, const RS_Vector& p2) {
	double width = p2.x - p1.x;
	double height = (p1.y - baseY) + (p2.y - baseY);

	return width * height / 2.0;
}



/*! Calculates a distance
    \param _p1 first point
    \param _p2 second point
*/
/*double
RS_InfoArea::calcDistance(RS_Vector *_p1, RS_Vector *_p2)
{
	return mtGetDistance(_p1->getX(), _p1->getY(), _p2->getX(), _p2->getY());
}*/


// EOF
