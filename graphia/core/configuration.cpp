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

#include "configuration.h"
#include <QColor>
#include <QSettings>
#include <QDesktopWidget>
#include <QApplication>

CConfiguration::CConfiguration()
{  
  //update list of available diagram
  config_diagram_desc diagram;
  diagram.name = "flow-diagram";
  diagram.version = CVersion(1, 0, 0);
  m_listPossibleDiagram.append(diagram);  
}

CConfiguration::~CConfiguration()
{

}

void CConfiguration::readSettings()
{
  QSettings config("graphia", "graphia");
  QSize fallbackSize;
  QPoint fallbackPoint;
 
  getMainWindowFallBackData(&fallbackPoint, &fallbackSize);
  
  config.beginGroup("MainWindow");
  m_mainWindowPosition = config.value("pos", fallbackPoint).toPoint();
  m_mainWindowSize = config.value("size", fallbackSize).toSize(); 
  config.endGroup();
}

void CConfiguration::getMainWindowFallBackData(QPoint* pos, QSize* size)
{
  QDesktopWidget *desktop = QApplication::desktop(); 
  int screenWidth; 
  int screenHeight;
  int x, y;

  size->setWidth(900);
  size->setHeight(486);
  
  // center the window when no setting exist
  screenWidth = desktop->width();
  screenHeight = desktop->height();
  x = (screenWidth - size->width()) / 2;
  y = (screenHeight - size->height()) / 2;
  y -= 50;
  
  if (x<0) x = 0;
  if (y<0) y = 0;

  pos->setX(x);
  pos->setY(y);
}

void CConfiguration::writeSettings()
{
  QSettings config("graphia", "graphia");

  config.beginGroup("MainWindow");
  config.setValue("pos", m_mainWindowPosition);
  config.setValue("size", m_mainWindowSize);
  config.endGroup();
  //m_mainWindowPosition = config.value("pos", ).toPoint();
  //m_mainWindowSize = config.value("size", ).toSize();

}

void CConfiguration::getMainWindowSetting(QPoint* pos, QSize* size)
{
  *pos = m_mainWindowPosition;
  *size = m_mainWindowSize;
}

void CConfiguration::setMainWindowSetting(const QPoint& pos, const QSize& size)
{
  m_mainWindowPosition = pos;
  m_mainWindowSize = size;
}

