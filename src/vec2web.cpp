/*****************************************************************************
**  $Id: vec2web.cpp,v 1.8 2002/11/10 15:07:41 andrew23 Exp $
**
**  This is part of the vec2web tool
**  Copyright (C) 2000 Andrew Mustun, Causeway Technologies
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License (version 2) as
**  published by the Free Software Foundation.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
******************************************************************************/

#include "vec2web.h"

#include "graphicview.h"

#include "dl_creationinterface.h"
#include "dl_dxf.h"

#include "rs_creation.h"
#include "rs_color.h"
#include "rs_graphic.h"
#include "rs_import.h"
#include "rs_system.h"

/*
extern "C" {
#include <gd.h>
#include <gdfontl.h>
#include <g2.h>
#include <g2_PS.h>
#include <g2_X11.h>
}
*/

#include <qimage.h>

#include <ctype.h>


/**
 * Default constructor.
 */
Vec2Web::Vec2Web () {
    strcpy(inputFile, "");
    strcpy(outputFile, "");
    maxSize.set(200,200);
    size.set(200,200);
    scaleUp = true;
    factor = 1.0;
    offset.set(0,0);

	RS_SYSTEM->init("vec2web", "vec2web");
}


/**
 * Converts inputFile to outputFile.
 */
void Vec2Web::convert() {
    // Read the DXF into graphic object
    RS_Import import(graphic);
    import.fileImport(inputFile);

    //cout << graphic;
    //getchar();

    // View in x11 if no outputfile was given.
    if (strlen(outputFile)==0) {
        output("X11");
        getchar();
        return;
    }

    int len = strlen(outputFile);
	int i=len-2;
	while (i>0 && outputFile[i]!='.') {
		--i;
	}

	char extension[16];
	strncpy(extension, &outputFile[i+1], 15);
	for (i=0; i<(int)strlen(extension); ++i) {
		extension[i] = toupper(extension[i]);
	}

	output(extension);
}


/**
 * Outputs an image from the graphic in the format descibed by 'format'.
 */
bool Vec2Web::output(const char* format) {
    bool ret=true;

    size.set(graphic.getMax().x - graphic.getMin().x,
             graphic.getMax().y - graphic.getMin().y);

    if (scaleUp || size.x>maxSize.x) {
        factor = maxSize.x / size.x;
	}
    if (scaleUp || size.y>maxSize.y) {
        factor = min(maxSize.y / size.y, factor);
	}

    size *= factor;
    size += RS_Vector(20,20);

    offset.set(10 - (int)(graphic.getMin().x * factor),
               10 - (int)(graphic.getMin().y * factor));

	outputQt(format);

    return ret;
}


/**
 * Outputs a png image from the graphic.
 */
bool Vec2Web::outputQt(const char* format) {
	GraphicView gv((int)maxSize.x, (int)maxSize.y);
	gv.setContainer(&graphic);
	gv.zoomAuto();
	gv.drawEntity(&graphic, false, true);
	QPixmap* buffer = gv.getBuffer();

	if (buffer!=NULL) {
		QImageIO iio;
		QImage img;
		img = *buffer;
		iio.setImage(img);
		iio.setFileName(outputFile);
		iio.setFormat(format);
        if (iio.write()) {
			return true;
		}
	}

	return false;
}


/**
 * Outputs the graphic into a format supported by the g2 library.
 *
 * \param handle The format handle of g2 created by a 2_open_* method.
 *               e.g. int handle = g2_open_GIF("simple.gif", 100, 100);
 */
//bool Vec2Web::outputG2(const char* format) {
	/*
    if (format!=F_GIF && format!=F_X11 && format!=F_PS && format!=F_WIN)
        return false;
	*/

	/*
    int handle = -1;			// handle which identifies the image for g2

    switch (format) {
    case F_GIF:
        //handle = g2_open_GIF(outputFile, (int)size.x, (int)size.y);
        break;

    case F_PS:
        handle = g2_open_PS(outputFile, g2_A4, g2_PS_land);
        break;

    case F_X11:
        handle = g2_open_X11((int)size.x, (int)size.y);
        break;

    case F_WIN:
        //handle = g2_open_win32((int)size.x, (int)size.y, outputFile, 0);
        break;

    default:
        handle = -1;
        break;
    }

    if (handle==-1)
        return false;

    for (RS_Entity* e=graphic.firstEntity(); e!=0; e=graphic.nextEntity()) {
        switch (e->rtti()) {
        case RS::EntityLine: {
                RS_Line* l = (RS_Line*)e;

                g2_line(handle,
                        transformX(l->getStartpoint().x),
                        transformY(l->getStartpoint().y),
                        transformX(l->getEndpoint().x),
                        transformY(l->getEndpoint().y));
            }
            break;

        case RS::EntityArc: {
                RS_Arc* a = (RS_Arc*)e;

                g2_arc(handle,
                       transformX(a->getCenter().x),
                       transformY(a->getCenter().y),
                       transformD(a->getRadius()),
                       transformD(a->getRadius()),
                       a->getAngle1()*ARAD, a->getAngle2()*ARAD);
            }
            break;

        case RS::EntityCircle: {
                RS_Circle* a = (RS_Circle*)e;

                g2_ellipse(handle,
                           transformX(a->getCenter().x),
                           transformY(a->getCenter().y),
                           transformD(a->getRadius()),
                           transformD(a->getRadius()));
            }
            break;

        default:
            break;
        }
    }

    if (format!=F_X11 && format!=F_WIN)
        g2_close(handle);

	*/

//    return true;
//}


/**
 * Transforms an x coordinate in the graphic to a x cordinate in a bitmap.
 */
double
Vec2Web::transformX (double x) {
    return (x * factor + offset.x);
}


/**
 * Transforms an y coordinate in the graphic to a y cordinate in a bitmap.
 *
 * \param swap Swap the coordinate (used if the bitmap has the zero on the top)
 */
double
Vec2Web::transformY (double y, bool swap) {
    return swap ? (size.y - (y * factor + offset.y)) :
           (y * factor + offset.y);
}


/**
 * Transforms a distance in the graphic to a distance in a bitmap.
 */
double
Vec2Web::transformD (double d) {
    return (d * factor);
}


// EOF
