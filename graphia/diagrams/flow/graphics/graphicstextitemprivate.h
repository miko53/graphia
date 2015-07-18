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

#ifndef GRAPHICSTEXTITEMPRIVATE_H
#define GRAPHICSTEXTITEMPRIVATE_H

#include <QObject>

class CGraphicsTextItem;

class CGraphicsTextItemPrivate : public QObject
{
  Q_OBJECT
public:
    explicit CGraphicsTextItemPrivate(CGraphicsTextItem* textItem, QObject *parent = 0);


private:
    CGraphicsTextItem* m_textItem;

signals:

public slots:
    void updateGeometry(int, int, int);

};

#endif // GRAPHICSTEXTITEMPRIVATE_H
