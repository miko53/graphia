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
#include "propertybrowser.h"
#include "graphia.h"
#include <QVBoxLayout>
#include <QDebug>


CPropertyBrowser::CPropertyBrowser(QWidget *parent) : QWidget(parent)
{
  m_ignorePropertySignal = false;
  m_propertyBrowser = new QtTreePropertyBrowser();
  m_propertyBrowser->setRootIsDecorated(true);
  
  m_propertyManager = new QtVariantPropertyManager();
  connect(m_propertyManager, SIGNAL(valueChanged(QtProperty*,QVariant)), this, SLOT(onPropertyValueChanged(QtProperty*,QVariant)));
  QtVariantEditorFactory* variantFactory = new QtVariantEditorFactory();
  m_propertyBrowser->setFactoryForManager(m_propertyManager, variantFactory);
  
  m_readOnlyManager = new QtVariantPropertyManager();  
  
  QVBoxLayout* layout = new QVBoxLayout;

  layout->setMargin(0);
  layout->addWidget(m_propertyBrowser);
  setLayout(layout);
}


void CPropertyBrowser::onSelectionChange(QList<QGraphicsItem *>& selectedItems)
{
  qDebug("CPropertyBrowser::OnSelectionChange");

  //First, disconnect from previous connected
  connectSelectedItems(false);

  //and reset all properties previously inserted
  resetPropertyList();
  m_selectedItems.clear();

  //now we can add the new properties (TODO)
  if (selectedItems.count() == 1)
  {
    //get properties of the items
    CGraphicsItem* currentItem = dynamic_cast<CGraphicsItem*>(selectedItems.at(0));

    if (currentItem != NULL)
    {
      QList<CPropertyDefinition::Type> listOfProperties =
          currentItem->getListOfGraphicalProperties();

      foreach(CPropertyDefinition::Type type, listOfProperties)
      {
        createProperty(type, currentItem);
      }

      m_selectedItems = selectedItems;
      //now connect the newly selected items to catch the change (e.g. position...)
      connectSelectedItems(true);
    }
    else
    {
      qDebug("CPropertyBrowser::onSelectionChange, currentItem == NULL, selection ignored");
    }
  }
  else
  {
    //TODO: select the common part according to type of selected items
  }

}


void CPropertyBrowser::connectSelectedItems(bool bConnect)
{
  foreach(QGraphicsItem* item, m_selectedItems)
  {
    CGraphicsItem* i =  dynamic_cast<CGraphicsItem*>(item);

    if (bConnect == true)
    {
      connect(i, SIGNAL(itemChanged(CGraphicsItem*,CPropertyDefinition::Type,QVariant)),
              this, SLOT(onItemChange(CGraphicsItem*,CPropertyDefinition::Type,QVariant)));
    }
    else
    {
      disconnect(i, SIGNAL(itemChanged(CGraphicsItem*,CPropertyDefinition::Type,QVariant)),
                 this, SLOT(onItemChange(CGraphicsItem*,CPropertyDefinition::Type,QVariant)));
    }
  }
}

void CPropertyBrowser::resetPropertyList(void)
{
  // remove the item is the equivalent of delete it
  foreach(QtProperty* prop, m_idToProperties)
  {
    delete prop;
  }
  m_idToProperties.clear();
  m_propertyToId.clear();
}

void CPropertyBrowser::createProperty(CPropertyDefinition::Type type, CGraphicsItem* item)
{
  QtVariantProperty* property;
  QtBrowserItem* browserItemAdded;

  switch (type)
  {
    case CPropertyDefinition::NAME:
    property = m_readOnlyManager->addProperty(QVariant::String, tr("Name"));
    m_idToProperties.insert(CPropertyDefinition::NAME, property);
    m_propertyToId.insert(property, CPropertyDefinition::NAME);

    property->setValue(item->getGraphicalProperties(CPropertyDefinition::NAME));
    break;

    case CPropertyDefinition::POS:
    property = m_propertyManager->addProperty(QVariant::PointF, tr("Position"));
    m_idToProperties.insert(CPropertyDefinition::POS, property);
    m_propertyToId.insert(property, CPropertyDefinition::POS);

    property->setValue(item->getGraphicalProperties(CPropertyDefinition::POS));
    break;

   case CPropertyDefinition::RADIUS:
    property = m_propertyManager->addProperty(QVariant::Double, tr("Radius"));
    m_idToProperties.insert(CPropertyDefinition::RADIUS, property);
    m_propertyToId.insert(property, CPropertyDefinition::RADIUS);

    property->setValue(item->getGraphicalProperties(CPropertyDefinition::RADIUS));
    break;

    case CPropertyDefinition::FONT:
    property = m_propertyManager->addProperty(QVariant::Font, tr("Font"));
    m_idToProperties.insert(CPropertyDefinition::FONT, property);
    m_propertyToId.insert(property, CPropertyDefinition::FONT);

    property->setValue(item->getGraphicalProperties(CPropertyDefinition::FONT));
    break;

    case CPropertyDefinition::FONT_COLOR:
    property = m_propertyManager->addProperty(QVariant::Color, tr("Font Color"));
    m_idToProperties.insert(CPropertyDefinition::FONT_COLOR, property);
    m_propertyToId.insert(property, CPropertyDefinition::FONT_COLOR);

    property->setValue(item->getGraphicalProperties(CPropertyDefinition::FONT_COLOR));
    break;

    case CPropertyDefinition::LINE_COLOR:
    property = m_propertyManager->addProperty(QVariant::Color, tr("Line Color"));
    m_idToProperties.insert(CPropertyDefinition::LINE_COLOR, property);
    m_propertyToId.insert(property, CPropertyDefinition::LINE_COLOR);

    property->setValue(item->getGraphicalProperties(CPropertyDefinition::LINE_COLOR));
    break;

    case CPropertyDefinition::BACKGND_COLOR:
    property = m_propertyManager->addProperty(QVariant::Color, tr("Backgnd. Color"));
    m_idToProperties.insert(CPropertyDefinition::BACKGND_COLOR, property);
    m_propertyToId.insert(property, CPropertyDefinition::BACKGND_COLOR);

    property->setValue(item->getGraphicalProperties(CPropertyDefinition::BACKGND_COLOR));
    break;

    case CPropertyDefinition::ALIGNMENT_H:
    property = m_propertyManager->addProperty(QtVariantPropertyManager::enumTypeId(), tr("Horiz. Alignment"));

    m_idToProperties.insert(CPropertyDefinition::ALIGNMENT_H, property);
    m_propertyToId.insert(property, CPropertyDefinition::ALIGNMENT_H);

    property->setAttribute("enumNames", CPropertyDefinition::m_HorizAlignmentName);
    property->setValue(item->getGraphicalProperties(CPropertyDefinition::ALIGNMENT_H));
    break;

    case CPropertyDefinition::ALIGNMENT_V:
    property = m_propertyManager->addProperty(QtVariantPropertyManager::enumTypeId(), tr("Vert. Alignment"));

    m_idToProperties.insert(CPropertyDefinition::ALIGNMENT_V, property);
    m_propertyToId.insert(property, CPropertyDefinition::ALIGNMENT_V);

    property->setAttribute("enumNames", CPropertyDefinition::m_VertAlignmentName);
    property->setValue(item->getGraphicalProperties(CPropertyDefinition::ALIGNMENT_V));
    break;

    case CPropertyDefinition::OFFSET_X:
    property = m_propertyManager->addProperty(QVariant::Double, tr("Offset X"));

    m_idToProperties.insert(CPropertyDefinition::OFFSET_X, property);
    m_propertyToId.insert(property, CPropertyDefinition::OFFSET_X);

    property->setValue(item->getGraphicalProperties(CPropertyDefinition::OFFSET_X));
    break;

    case CPropertyDefinition::OFFSET_Xb:
    property = m_propertyManager->addProperty(QVariant::Double, tr("Offset X left"));

    m_idToProperties.insert(CPropertyDefinition::OFFSET_Xb, property);
    m_propertyToId.insert(property, CPropertyDefinition::OFFSET_Xb);

    property->setValue(item->getGraphicalProperties(CPropertyDefinition::OFFSET_Xb));
    break;

    case CPropertyDefinition::OFFSET_Y:
    property = m_propertyManager->addProperty(QVariant::Double, tr("Offset Y"));

    m_idToProperties.insert(CPropertyDefinition::OFFSET_Y, property);
    m_propertyToId.insert(property, CPropertyDefinition::OFFSET_Y);

    property->setValue(item->getGraphicalProperties(CPropertyDefinition::OFFSET_Y));
    break;
    
    case CPropertyDefinition::HEIGHT:
    property = m_propertyManager->addProperty(QVariant::Double, tr("Height"));

    m_idToProperties.insert(CPropertyDefinition::HEIGHT, property);
    m_propertyToId.insert(property, CPropertyDefinition::HEIGHT);

    property->setValue(item->getGraphicalProperties(CPropertyDefinition::HEIGHT));
    break;
     
    case CPropertyDefinition::WIDTH:
    property = m_propertyManager->addProperty(QVariant::Double, tr("Width"));

    m_idToProperties.insert(CPropertyDefinition::WIDTH, property);
    m_propertyToId.insert(property, CPropertyDefinition::WIDTH);

    property->setValue(item->getGraphicalProperties(CPropertyDefinition::WIDTH));
    break;
    
    case CPropertyDefinition::CLASS:
    property = m_propertyManager->addProperty(QtVariantPropertyManager::enumTypeId(), tr("Variable type"));

    m_idToProperties.insert(CPropertyDefinition::CLASS, property);
    m_propertyToId.insert(property, CPropertyDefinition::CLASS);
    
    property->setAttribute("enumNames", *(getApp()->getDocument()->getItemFactory()->getTypeFactory()->getListOfName()));
    property->setValue(item->getGraphicalProperties(CPropertyDefinition::CLASS));
    break;
    
    case CPropertyDefinition::FLOW_NAME:
    property = m_readOnlyManager->addProperty(QVariant::String, tr("Flow name"));
    m_idToProperties.insert(CPropertyDefinition::FLOW_NAME, property);
    m_propertyToId.insert(property, CPropertyDefinition::FLOW_NAME);
    property->setValue(item->getGraphicalProperties(CPropertyDefinition::FLOW_NAME));
    break;
    
    case CPropertyDefinition::DISPLAY_LABEL:
    property = m_propertyManager->addProperty(QVariant::Bool, tr("display flow name"));

    m_idToProperties.insert(CPropertyDefinition::DISPLAY_LABEL, property);
    m_propertyToId.insert(property, CPropertyDefinition::DISPLAY_LABEL);

    property->setValue(item->getGraphicalProperties(CPropertyDefinition::DISPLAY_LABEL));
    break;
      
    default:
    qDebug("Properties not yet managed be patient ;-)");
    break;
  }
  
  browserItemAdded = m_propertyBrowser->addProperty(property);
  m_propertyBrowser->setExpanded(browserItemAdded, false);
}



void CPropertyBrowser::onItemChange(CGraphicsItem *item, CPropertyDefinition::Type param, QVariant value)
{
  Q_UNUSED(item);
  //qDebug("CPropertyBrowser::OnItemChange");
  QtVariantPropertyManager* manager;
  
  if (!m_idToProperties.contains(param))
  {
    qDebug("unkown param, ignore it (%i)", param);
    return;
  }
  
  switch (param)
  {
    case CPropertyDefinition::FLOW_NAME:
    case CPropertyDefinition::NAME:
      manager = m_readOnlyManager;
      break;
      
    default:
      manager = m_propertyManager;
      break;
  }
  
  m_ignorePropertySignal = true; //need to avoid setting parameter after user operation, useless
  manager->setValue(m_idToProperties[param], value);
  m_ignorePropertySignal = false; //need to avoid setting parameter after user operation, useless
}

void CPropertyBrowser::onPropertyValueChanged(QtProperty *property, QVariant value)
{
  //qDebug("CPropertyBrowser::onPropertyValueChanged");

  if ((m_selectedItems.count() == 0) ||  // when property has added a signal is emitted, in this case, it is ignored
      (m_ignorePropertySignal == true))  // other case when signal must be ignored
  {
    //qDebug("spurious signal ignore it (%i) (%i)",
    //       m_selectedItems.count(),
    //       m_ignorePropertySignal);
    return;
  }

  if (!m_propertyToId.contains(property))
  {
    qDebug("sub property not manager, ignore it");
    return;
  }


  //qDebug() << property->propertyName();

  //now update items with the new value
  if (m_selectedItems.count() == 1)
  {
    //qDebug("TO UPDATE");
    CGraphicsItem* currentItem = dynamic_cast<CGraphicsItem*>(m_selectedItems.at(0));

    currentItem->setGraphicalProperties(m_propertyToId.value(property), value);
  }
  else
  {
    Q_ASSERT(false); //TODO: to manage multiple selections
  }
}

CPropertyBrowser::~CPropertyBrowser()
{
  qDebug("~CPropertyBrowser");
}


