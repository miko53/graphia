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
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include "document.h"
#include "toolbox/toolbox.h"
#include "mainwindow.h"
#include "diagrams/flow/graphics/graphicsscene.h"

class CMainWidget : public QWidget
{
  Q_OBJECT

public:
  CMainWidget(CMainWindow* wind, QWidget* parent = 0, Qt::WindowFlags f = 0);
  virtual ~CMainWidget();
  
  typedef enum
  {
    ZOOM_11,
    ZOOM_IN,
    ZOOM_OUT
  } zoomType;
  
  void zoom(zoomType zoomValue);

protected:
  void disconnectEvents();
  void connectEvents(CGraphicsScene* newScene);

protected:
  CMainWindow* m_mainWind;

  //CDocument* m_doc;
  QTabWidget* m_tabWidget;
  //CToolBox* m_toolBox;
  CGraphicsScene* m_currentScene;
    
signals:
  void changeTypeOfDiagram(CGraphicsScene* diagram);
  void selectionChanged(QList<QGraphicsItem *>& selectedItems);

public slots:
  void displayDiagram(CGraphicsScene* scene);
  void closeDiagram(CGraphicsScene* scene);
  void resetDisplay();
  
protected slots:
  void onTabChanged(int i);
  void onSelectionChanged(void);
  void onTabCloseRequested(int i);
  
  
};

#endif // MAINWIDGET_H
