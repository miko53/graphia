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
#ifndef TOOLGROUP_H
#define TOOLGROUP_H

#include <QMap>
#include <QString>
#include "toolitem.h"

class CToolGroup
{
  public:
    CToolGroup(const QString& name);
    virtual ~CToolGroup();
    void insertToolItem(CToolItem* toolItem);

    int getNbOfItems();
    CToolItem* getAt(int i);
    const QString& name() { return m_name; };
    
  protected:
    QString m_name;
    QVector<CToolItem*> m_listToolItem;
  private:
    
};


#endif /* TOOLGROUP_H */
