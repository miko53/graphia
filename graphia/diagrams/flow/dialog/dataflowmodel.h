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


#ifndef DATAFLOWMODEL_H
#define DATAFLOWMODEL_H

#include <QAbstractTableModel>
#include "diagrams/flow/model/datafactory.h"


class CDataFlowModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  explicit CDataFlowModel(CDataFactory* factory, QObject* parent = 0);
  
  virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
  virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
  
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const; 
  
  CDataFactory* getDataFactory() { return m_dataFactory;}
  
  void update();
  
protected:
  CDataFactory* m_dataFactory;
  
};

#endif // DATAFLOWMODEL_H
