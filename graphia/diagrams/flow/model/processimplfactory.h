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


#ifndef PROCESSIMPLFACTORY_H
#define PROCESSIMPLFACTORY_H

#include "processimplementation.h"
#include <QList>

class CProcessImplFactory
{
public:
  CProcessImplFactory();
  virtual ~CProcessImplFactory();
  
  CProcessImplementation* createNewImplementationProcess(const QString& name);
  bool renameProcess ( CProcessImplementation* item, QString newName );
  CProcessImplementation* getImplementationProcess ( QString name );
  CProcessImplementation* getImplementationProcess ( int ID );  

  int getCount() { return m_listProcess.count(); }
  CProcessImplementation* getAt(int index) { return m_listProcess.at(index); }
  
  void insertImplementationProcess(CProcessImplementation* p, int id = -1);
  CItem* search ( int ID );
  
protected:
 CItem* ifAlreadyExists ( const QString& name );

  
protected:
  QList<CProcessImplementation*> m_listProcess;
  int m_nextID;
};

#endif // PROCESSIMPLFACTORY_H
