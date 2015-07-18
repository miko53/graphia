/*
    graphia - little tool for building designer diagram
    Copyright (C) 2012  Mickael Sergent

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef GRAPHICSCIRCLEWRITER_H
#define GRAPHICSCIRCLEWRITER_H

#include "graphicscircle.h"
#include "graphicsarrow.h"
#include "graphicsrect.h"
#include "graphicsdoubleline.h"
#include "graphicsinput.h"
#include "graphicsoutput.h"
#include "graphicstextitem.h"
#include <QDomDocument>
#include <QDomElement>

class CGraphicsWriter
{
public:
  CGraphicsWriter( CGraphicsCircle* item );
  CGraphicsWriter( CGraphicsArrow* item );
  CGraphicsWriter( CGraphicsRect* item );
  CGraphicsWriter( CGraphicsDoubleLine* item );
  CGraphicsWriter( CGraphicsInput* item);
  CGraphicsWriter( CGraphicsOutput* item);
  CGraphicsWriter( CGraphicsTextItem* item);
  virtual ~CGraphicsWriter();

  void buildDomDocument(QDomDocument& doc, QDomElement& element);
  
  
protected:
  void buildDomDocumentOfCircle(QDomDocument& doc, QDomElement& gobj);
  void buildDomDocumentOfArrow(QDomDocument& doc, QDomElement& gobj);
  void buildDomDocumentOfTerminator(QDomDocument& doc, QDomElement& gobj);
  void buildDomDocumentOfStore(QDomDocument& doc, QDomElement& gobj);
  void buildDomDocumentOfInput(QDomDocument& doc, QDomElement& gobj);
  void buildDomDocumentOfOutput(QDomDocument& doc, QDomElement& gobj);
  void buildDomDocumentOfTextItem(QDomDocument& doc, QDomElement& gobj);

  enum
  {
    CIRCLE,
    ARROW,
    TERMINATOR,
    STORE,
    INPUT,
    OUTPUT,
    TEXTITEM
  };
  
  typedef union
  {
    CGraphicsCircle* circleItem;
    CGraphicsArrow* arrowItem;
    CGraphicsRect* terminatorItem;
    CGraphicsDoubleLine* storeItem;
    CGraphicsInput* inputItem;
    CGraphicsOutput* outputItem;
    CGraphicsTextItem* textItem;
  } graphicalItemPtr;
  
  int m_type;
  graphicalItemPtr m_item;
};

#endif // GRAPHICSCIRCLEWRITER_H
