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
#ifndef MAINWINDOW_P_H
#define MAINWINDOW_P_H

#include <QObject>
#include <QHash>
#include <QDir>
#include <QAction>
#include <QDockWidget>
#include "diagrams/generic/toolitemmodel.h"
#include "document.h"
#include "toolbox/toolbox.h"
#include "application.h"
#include "property/propertybrowser.h"

class CMainWidget;
class CMainWindow;

class CMainWindowPrivate : public QObject
{
    Q_OBJECT
public:
  explicit CMainWindowPrivate(CMainWindow *mainWindow, CApplication* app);
  ~CMainWindowPrivate();

  CToolBox* getToolBox() { return m_toolBox; }
  CApplication* getApp() { return m_app; }
  CDocument* getDoc()  { return m_app->getDocument(); }
  CPropertyBrowser* getPropertyBrowser() { return m_propertyBrowser; }
  CMainWidget* createMainWidget();
  
  void setupAction();
  void setupMenu();
  void setupToolBox();
  void setupPropertyBox();
  void setupToolBar();
  void setupStatusBar();

  void createNewFile();

signals:

public slots:
  //File menu
  void onNewFile();
  void open();
  void save();
  void saveAs();
  
  void addNewDiagram();
  void displayListOfDiagram();
  void displayListOfDataTypes();
  
  void displayToolBox();
  void displayPropertyBox();
  void displayAboutWnd();
  
  void onDocumentModified(bool isModified);
  void onDisplayDiagram ( CGraphicsScene* scene );
  
  void onZoomNormal();
  void onZoomIn();
  void onZoomOut();

private:
  void setupActionFile();
  void setupActionEdit();
  void setupActionDisplay();
  void setupActionDiagram();
  void setupActionWindow();
  void setupActionHelp();

  void setupMenuFile();
  void setupMenuEdit();
  void setupMenuDisplay();
  void setupMenuDiagram();
  void setupMenuWindow();
  void setupMenuHelp();
  
  void readFile();  
  void buildName();
  void setDocumentTitle( const QString& filename );
  
private :
  CMainWindow* m_mainWindow;
  CApplication* m_app;
  QHash< QString, QAction*> m_hashAction;
  QDockWidget *m_dockOfToolBox;
  QDockWidget* m_dockOfPropertyBrowser;
  CToolBox* m_toolBox;
  CPropertyBrowser* m_propertyBrowser;
  CMainWidget* m_mainWidget;
  
  QString m_displayName;
  QString m_fullFileName;
  bool m_hasUntitledName;
  QDir m_currentDir;
  int m_untitledDocumentNumber;
};

#endif // MAINWINDOW_P_H
