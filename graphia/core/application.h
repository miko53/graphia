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
#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include "diagrams/generic/toolitemmodel.h"
#include "document.h"
#include "configuration.h"

class CApplication : public QApplication
{
public:
  CApplication(int& argc, char** argv);
  virtual ~CApplication();

  CToolItemModel* getModel() { return m_model; }
  CDocument* getDocument() { return m_doc; }
  CConfiguration* getCfg() { return m_cfg; }
  CGraphicsConfiguration* getGraphicsConfig() { return m_cfg->getGraphicsConfiguration(); }

protected:
  CToolItemModel* m_model;
  CDocument* m_doc;
  CConfiguration* m_cfg;
 
};

#endif // APPLICATION_H
