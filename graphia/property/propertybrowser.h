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
#ifndef PROPERTYBROWSER_H
#define PROPERTYBROWSER_H

#include "QtTreePropertyBrowser"
#include "QtVariantPropertyManager"
#include "diagrams/generic/graphicsitem.h"
#include "propertydefinition.h"


class CPropertyBrowser : public QWidget
{
  Q_OBJECT

public:  
  CPropertyBrowser(QWidget* parent = NULL);
  virtual ~CPropertyBrowser();

public slots:
  void onSelectionChange(QList<QGraphicsItem*> &selectedItems);
  void onItemChange(CGraphicsItem *item, CPropertyDefinition::Type param, QVariant value);
  void onPropertyValueChanged(QtProperty* property, QVariant value);

private:
  void connectSelectedItems(bool bConnect);
  void resetPropertyList(void);
  void createProperty(CPropertyDefinition::Type type, CGraphicsItem* item);

protected:
  QtTreePropertyBrowser* m_propertyBrowser;
  QtVariantPropertyManager* m_propertyManager;
  QtVariantPropertyManager* m_readOnlyManager;

  QList<QGraphicsItem*> m_selectedItems;
  QMap<CPropertyDefinition::Type, QtProperty *> m_idToProperties;
  QMap<QtProperty*, CPropertyDefinition::Type> m_propertyToId;
  bool m_ignorePropertySignal;
};

#endif // PROPERTYBROWSER_H
