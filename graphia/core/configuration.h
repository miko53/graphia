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


#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "version.h"
#include "diagrams/generic/graphicsconfiguration.h"

class CConfiguration
{
public:
  CConfiguration();
  virtual ~CConfiguration();
  
  void setGraphicsConfiguration(const CGraphicsConfiguration& gConfig) { m_graphicConfiguration = gConfig; }
  CGraphicsConfiguration* getGraphicsConfiguration() { return &m_graphicConfiguration; }
  
  void readSettings();
  void writeSettings();
  
  void getMainWindowSetting(QPoint* pos, QSize* size);
  void setMainWindowSetting(const QPoint& pos, const QSize& size);
  
  int getListDiagramCount() { return m_listPossibleDiagram.count(); }
  QString getDiagramNameAt(int index) { return m_listPossibleDiagram.at(index).name; }
  QString getDiagramVersionAt(int index) { return (m_listPossibleDiagram.at(index).version).getVersionString();}

protected:
  void getMainWindowFallBackData(QPoint* pos, QSize* size);
  
  QPoint m_mainWindowPosition;
  QSize m_mainWindowSize;
  
  typedef struct
  {
    QString name;
    CVersion version;
  } config_diagram_desc;
  QList<config_diagram_desc> m_listPossibleDiagram;
  
  CGraphicsConfiguration m_graphicConfiguration;
  
private:
  
};

#endif // CONFIGURATION_H
