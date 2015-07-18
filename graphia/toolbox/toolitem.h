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
#ifndef TOOLITEM_H
#define TOOLITEM_H

#include <QString>
#include <QIcon>

class CToolItem
{
  public:
    CToolItem(const QString& name, int id, const QIcon& icon1, const QIcon& icon2, void* graphicItem);
    virtual ~CToolItem();

    const QString& name() { return m_name; }
    int id() { return m_id; }
    const QIcon& icon1() { return m_icon1; }    
    const QIcon& icon2() { return m_icon2; }
    bool isDefault() { return m_isDefault; }
    void setDefault(bool d) { m_isDefault = d; }
    
  protected:
    QString m_name;
    int     m_id;
    QIcon   m_icon1;
    QIcon   m_icon2;
    void*   m_graphicItem;
    bool    m_isDefault;
  private:
};


#endif /* TOOLITEM_H */