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
// main of application

#include "application.h"
#include "mainwindow.h"

CApplication* gApp;

CApplication* getApp()
{
  Q_ASSERT(gApp != NULL);
  return gApp;
}


int main(int argc, char* argv[])
{
  gApp = new CApplication(argc, argv);
  
  //AApplication application(argc, argv);
  CMainWindow* window = new CMainWindow(gApp);
  
  window->show();
  
  return gApp->exec();
}
