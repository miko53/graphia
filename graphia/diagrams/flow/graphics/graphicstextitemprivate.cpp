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

#include "graphicstextitemprivate.h"
#include "graphicstextitem.h"
#include <QTextDocument>

CGraphicsTextItemPrivate::CGraphicsTextItemPrivate(CGraphicsTextItem* textItem, QObject *parent) :
    QObject(parent)
{
    m_textItem = textItem;

    connect(m_textItem->document(), SIGNAL(contentsChange(int, int, int)),
            this, SLOT(updateGeometry(int, int, int)));

}



void CGraphicsTextItemPrivate::updateGeometry(int, int, int)
{
  //qDebug("updateGeometry");
  m_textItem->setTextWidth(-1);
  m_textItem->setTextWidth(m_textItem->boundingRect().width());
  m_textItem->updateAlignment();
}


