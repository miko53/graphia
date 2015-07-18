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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CToolBox;
class CApplication;
class CPropertyBrowser;
class CDocument;
class CGraphicsScene;

class CMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit CMainWindow(CApplication* app, QWidget *parent = 0);
  virtual ~CMainWindow();

  CToolBox* getToolBox();
  CApplication* getApp();
  CDocument* getDoc();
  CPropertyBrowser* getPropertyBrowser();
  
  virtual void closeEvent(QCloseEvent* event);
    
protected:
  void setup();
  void changeSizeAndPosition();
  
signals:

public slots:
  void onDisplayDiagram(CGraphicsScene* scene);

private:
  class CMainWindowPrivate* d;
  friend class CMainWindowPrivate;
};

#endif // MAINWINDOW_H
