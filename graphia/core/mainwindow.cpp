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
#include "mainwindow.h"
#include "mainwindow_p.h"
#include "mainwidget.h"
#include "application.h"
#include <QCloseEvent>

CMainWindow::CMainWindow(CApplication* app, QWidget *parent) :  QMainWindow(parent)
{
  setWindowIcon(QIcon(":/flow_block.png"));
  d = new CMainWindowPrivate(this, app);

  setDockOptions(AnimatedDocks | AllowTabbedDocks | ForceTabbedDocks);

  setup();

  CMainWidget* mainwidget = d->createMainWidget();
  
  connect(getDoc(), SIGNAL(newDocumentCreated()), mainwidget, SLOT(resetDisplay()));
  connect(getDoc(), SIGNAL(displayNewDiagram(CGraphicsScene*)), mainwidget, SLOT(displayDiagram(CGraphicsScene*)));
  connect(getDoc(), SIGNAL(diagramToBeRemoved(CGraphicsScene*)), mainwidget, SLOT(closeDiagram(CGraphicsScene*)));
  
  d->createNewFile();
  
  setCentralWidget(mainwidget);
  
  changeSizeAndPosition();
}

void CMainWindow::changeSizeAndPosition()
{
  QPoint pos;
  QSize size;
  getApp()->getCfg()->getMainWindowSetting(&pos, &size);
  move(pos);
  resize(size);
}


void CMainWindow::setup()
{
  d->setupAction();
  d->setupMenu();
  d->setupToolBar();
  d->setupToolBox();
  d->setupPropertyBox();
  d->setupStatusBar();
}

CToolBox* CMainWindow::getToolBox()
{
  Q_ASSERT(d!=NULL);
  return d->getToolBox();
}

CApplication* CMainWindow::getApp()
{
  Q_ASSERT(d!=NULL);
  return d->getApp();
}

CDocument* CMainWindow::getDoc()
{
  Q_ASSERT(d!=NULL);
  return d->getApp()->getDocument();
}


CPropertyBrowser* CMainWindow::getPropertyBrowser()
{
  Q_ASSERT(d!=NULL);
  return d->getPropertyBrowser();
}

void CMainWindow::closeEvent(QCloseEvent* event)
{
  d->getApp()->getCfg()->setMainWindowSetting(pos(),
                                              size());
  d->getApp()->getCfg()->writeSettings();
 
 QMainWindow::closeEvent(event);
}

void CMainWindow::onDisplayDiagram ( CGraphicsScene* scene )
{
  d->onDisplayDiagram(scene);
}



CMainWindow::~CMainWindow()
{
  if (d)
    delete d;
  d = NULL;
}
