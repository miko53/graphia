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
#ifndef PROCESSCONTEXTFACTORY_H
#define PROCESSCONTEXTFACTORY_H

#include "processcontext.h"

class CProcessContextFactory
{
public:
  CProcessContextFactory();
  virtual ~CProcessContextFactory();
  
  CProcessContext* createNewContextProcess(const QString& name);
  
  int getCount() { return m_listProcess.count(); }
  CProcessContext* getAt(int index) { return m_listProcess.at(index); }
  void insertContextProcess ( CProcessContext* p, int id = -1 );
  CItem* search ( int ID );

protected:
  QList<CProcessContext*> m_listProcess;
  int m_nextID;  
};

#endif // PROCESSCONTEXTFACTORY_H
