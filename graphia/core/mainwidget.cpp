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
#include "mainwidget.h"
#include "application.h"
#include "property/propertybrowser.h"
#include "graphicsview.h"
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QPushButton>

CMainWidget::CMainWidget(CMainWindow* wind, QWidget* parent , Qt::WindowFlags f): QWidget(parent, f)
{
  m_mainWind = wind;
  m_currentScene = NULL;
  QVBoxLayout* layout = new QVBoxLayout();

  m_tabWidget = new QTabWidget();
  m_tabWidget->setTabsClosable(true);
  
  layout->addWidget(m_tabWidget);
  setLayout(layout);
  
  connect(m_tabWidget, SIGNAL(currentChanged(int)), SLOT(onTabChanged(int)));
  connect(m_tabWidget, SIGNAL(tabCloseRequested(int)), SLOT(onTabCloseRequested(int)));
  
  connect(this, SIGNAL(changeTypeOfDiagram(CGraphicsScene*)), m_mainWind->getToolBox(), SLOT(onChangeTypeOfDiagram(CGraphicsScene*)));
  connect(this, SIGNAL(changeTypeOfDiagram(CGraphicsScene*)), m_mainWind, SLOT(onDisplayDiagram(CGraphicsScene*)));  
  connect(this, SIGNAL(selectionChanged(QList<QGraphicsItem*>&)), m_mainWind->getPropertyBrowser(), SLOT(onSelectionChange(QList<QGraphicsItem*>&)));
}

void CMainWidget::resetDisplay()
{
  //bug> when newDocument is call and the display is reset all previous scene are delete before, so m_currentScene is no more relevant
  //qDebug("resetDisplay");
  m_currentScene = NULL;
  m_tabWidget->clear();
  
}

void CMainWidget::displayDiagram(CGraphicsScene* scene)
{
  CGraphicsView* gView;
  
  if (scene->isDisplayed() == true)
  {
    
    //search the scene and display it TODO faire fonction commune de recherche avec closeDiagram
    for(int i = 0; i < m_tabWidget->count(); i++)
    {
      gView = dynamic_cast<CGraphicsView*>(m_tabWidget->widget(i));
      if (scene == dynamic_cast<CGraphicsScene*>(gView->scene()))
      {
        m_tabWidget->setCurrentIndex(i);
      }
    }
  }
  else
  {
    gView = new CGraphicsView(scene);
    int i = m_tabWidget->addTab(gView, scene->getTitle());
    qDebug("addTab %i", i);
    scene->setDisplayState(true);
    m_tabWidget->setCurrentIndex(i);
  }
}

void CMainWidget::closeDiagram(CGraphicsScene* scene)
{
  CGraphicsView* view;
  //search the scene
  for(int i = 0; i < m_tabWidget->count(); i++)
  {
    view = dynamic_cast<CGraphicsView*>(m_tabWidget->widget(i));
    if (scene == dynamic_cast<CGraphicsScene*>(view->scene()))
    {
      qDebug("removeTab %i", i);
      m_tabWidget->removeTab(i);
      delete view;
      scene->setDisplayState(false);
    }
  }
}


void CMainWidget::onTabChanged(int i)
{
  CGraphicsView* newView = NULL;
  CGraphicsScene* newScene = NULL;
//  CDiagram* diagram = NULL;

  qDebug("onTabChanged %i", i);
  
  if (m_currentScene != NULL)
  {
    // disconnect old event
    disconnectEvents();
  }
  
  if (i == -1)
  {
    m_currentScene = NULL;
    emit changeTypeOfDiagram(NULL);
  }
  else
  {
    newView = (CGraphicsView*) m_tabWidget->widget(i);
    newScene = (CGraphicsScene*) newView->scene();
    if (newScene != NULL)
    {
      emit changeTypeOfDiagram(newScene);
      connectEvents(newScene);
    }
    else
    {
      qDebug("no conectEvents done, because no scene available");
    }
  }
}

void CMainWidget::disconnectEvents()
{
  m_currentScene->clearSelection();
  disconnect(m_mainWind->getToolBox(), SIGNAL(itemSelected(int,CToolButton::STATE)), m_currentScene, SLOT(onChangeItem(int,CToolButton::STATE)));
  disconnect(m_currentScene, SIGNAL(itemInserted()), m_mainWind->getToolBox(), SLOT(returnToSelectButton()));
  disconnect(m_currentScene, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
}

void CMainWidget::connectEvents(CGraphicsScene* newScene)
{
  newScene->resetSceneMode();
  connect(m_mainWind->getToolBox(), SIGNAL(itemSelected(int,CToolButton::STATE)), newScene, SLOT(onChangeItem(int,CToolButton::STATE)));
  connect(newScene, SIGNAL(itemInserted()), m_mainWind->getToolBox(), SLOT(returnToSelectButton()));
  connect(newScene, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
  m_currentScene = newScene; 
}


void CMainWidget::onTabCloseRequested(int i)
{
  qDebug("onTabCloseRequested %i", i);
  CGraphicsView* currentView;
  CGraphicsScene* currentScene;
  
  currentView = dynamic_cast<CGraphicsView*>(m_tabWidget->widget(i));
  Q_ASSERT(currentView != NULL);
  
  currentScene = dynamic_cast<CGraphicsScene*>(currentView->scene());
  Q_ASSERT(currentScene != NULL);
  
  //qDebug("removeTab %i", i);
  m_tabWidget->removeTab(i);
  currentScene->setDisplayState(false);  
  delete currentView;
}



void CMainWidget::onSelectionChanged()
{
  qDebug("CMainWidget::onSelectionChanged");

  QList<QGraphicsItem*> itemsList = m_currentScene->selectedItems();

/*  foreach(QGraphicsItem* item, itemsList)
  {
    qDebug("item->type() = %i", item->type());
    CGraphicsItem* i =  dynamic_cast<CGraphicsItem*>(item);
  }
*/

  emit selectionChanged(itemsList);
}

void CMainWidget::zoom (zoomType zoomValue )
{
  CGraphicsView* currentView;
  currentView = (CGraphicsView*) m_tabWidget->currentWidget();
  
  if (currentView != NULL)
  {
    switch (zoomValue)
    {
      case ZOOM_11:
        currentView->zoomNormal();
        break;
        
      case ZOOM_IN:
        currentView->zoomIn();
        break;
        
      case ZOOM_OUT:
        currentView->zoomOut();
        break;
 
      default:
        Q_ASSERT(false);
        break;
    }
  }
}


CMainWidget::~CMainWidget()
{

}
