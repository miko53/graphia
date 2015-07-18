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


#include "graphicsconfiguration.h"

CGraphicsConfiguration::CGraphicsConfiguration()
{
  m_sceneSize = QRectF(0,0, 3000, 3000);
  m_circleRadius = 25.0;
  m_arrowAngle = 30; //in deg
  m_arrowLength = 15.0;
  m_anchorSize = 5.0;
  m_storeHeight = 25.0;
  m_storeLength = 50.0;
  m_alignmentH = Qt::AlignHCenter;
  m_alignmentV = Qt::AlignVCenter;
  m_displayLabel = true;
  m_font =  QFont("Deja Vu Sans", 8);
  m_fontColor = QColor(Qt::black);
  m_lineColor = QColor(Qt::black);
  m_background = QColor(Qt::white);
}

CGraphicsConfiguration::~CGraphicsConfiguration()
{
}

connectorOffset CGraphicsConfiguration::offsets()
{
  connectorOffset r;
  r.offsetX = arrowLength() * cos(arrowAngle());
  r.offsetY = arrowLength() * sin(arrowAngle());
  //qDebug("r.offsetX = %g ; r.offsetY = %g", r.offsetX, r.offsetY);
  return r;
}
