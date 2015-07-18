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


#include "dataflowmodel.h"

CDataFlowModel::CDataFlowModel(CDataFactory* factory, QObject* parent): QAbstractTableModel(parent)
{
  Q_ASSERT(factory != NULL);
  m_dataFactory = factory;
}


int CDataFlowModel::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return m_dataFactory->getCount();
}

int CDataFlowModel::columnCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return 3;
}


QVariant CDataFlowModel::data(const QModelIndex& index, int role) const
{
  if (role == Qt::DisplayRole)
  {
    if (index.column() == 0)
    {
      return m_dataFactory->getDataNameAt(index.row());
    }
    else if (index.column() == 1)
    {
      return m_dataFactory->getTypeOfDataAt(index.row());
    }
    else if (index.column() == 2)
    {
      return m_dataFactory->getFlowTypeofDataAt(index.row());
    }
  }
  else if (role == Qt::UserRole)
  {
    return qVariantFromValue((void *) m_dataFactory->getDataAt(index.row()));
  }
  
  // return QVariant empty for the another role
  return QVariant();
}


QVariant CDataFlowModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if ((role == Qt::DisplayRole) && (orientation == Qt::Horizontal))
  {
    switch (section)
    {
      case 0:
        return QString(tr("Name"));
      case 1:
        return QString(tr("Type"));
      case 2: 
        return QString(tr("Flow type"));
      default:
        break;
    }
  }
  
  // return QVariant empty for the another role
  return QVariant();
}

void CDataFlowModel::update()
{
 //Not necessary ? why... emit dataChanged(index(0, 0), index(rowCount()-1, columnCount()-1));
  emit layoutChanged();
}
