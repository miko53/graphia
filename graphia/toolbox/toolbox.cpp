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
#include "toolbox.h"
#include "widgets/toolbutton.h"
#include "widgets/buttongroup.h"
#include <QToolBox>
#include <QLabel>

CToolBox::CToolBox(CToolItemModel* model, QWidget* parent, Qt::WindowFlags f) : QWidget(parent,f)
{
  m_model = model;
  QVBoxLayout* layout = new QVBoxLayout();
  m_btnGroup = new CButtonGroup();
  
  m_btnGroup->setExclusive(true);
  connect(m_btnGroup, SIGNAL(buttonClicked(int,CToolButton::STATE)),
          this, SLOT(onSelectButton(int,CToolButton::STATE)));

  createAllButtonsState(layout);  
  setLayout(layout);
  setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding));
}

void CToolBox::createAllButtonsState(QVBoxLayout* layout)
{
  //parse whole group and insert it into the toolbox
  //first add the common tool, always active :
  int nbGroup = m_model->getNbOfGroups();
  int i, j;
  int nbButton=0, col=0;
  QGridLayout* gridLayout;
  QToolBox* curToolBox;
  
  for (i = 0; i < nbGroup; i++)
  {
    CToolGroup* groupItm = m_model->getAt(i);
    gridLayout = new QGridLayout();

    if (i > 0)
    {
      curToolBox = new QToolBox();
    }
    
    for (j = 0; j < groupItm->getNbOfItems(); j++)
    {
      CToolButton* toolBtn;
      toolBtn = new CToolButton(groupItm->getAt(j)->icon1(),
                                groupItm->getAt(j)->icon2(),
                                groupItm->getAt(j)->name());
      //qDebug("groupItm->getAt(j)->id() = %i", groupItm->getAt(j)->id());
      m_btnGroup->addButton(toolBtn, groupItm->getAt(j)->id());
      gridLayout->addWidget(toolBtn, nbButton++ / 2, col++ % 2);
      
      if (i == 0)
        toolBtn->setEnabled(true);
      else
        toolBtn->setEnabled(false);
      
      if (groupItm->getAt(j)->isDefault())
      {
        toolBtn->setEnabled(true);
        toolBtn->setChecked(true);
      }
    }

    if (i > 0)
    {
      QWidget* wid = new QWidget();
      wid->setLayout(gridLayout);
      wid->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
      curToolBox->addItem(wid, groupItm->name());
      
      layout->addWidget(curToolBox);
    }
    else
    {
      layout->addLayout(gridLayout);
    }
  }
}

void CToolBox::resetButtonState()
{
  QAbstractButton* btn;
  int i;
  
  btn = m_btnGroup->button(0);
  btn->setChecked(true);
  
  //start from 2 because the first two buttons are always visible
  for (i = 2; i < CToolItemModel::ITEM_NB; i++)
  {
    btn = m_btnGroup->button(i);
    btn->setEnabled(false);
  }
}



void CToolBox::onChangeTypeOfDiagram(CGraphicsScene* newDiagram)
{
  QAbstractButton* btn;
  int i;
  int id;
  
  resetButtonState();
  
  if (newDiagram != NULL)
  {
    for (i = 0; i < newDiagram->getAutho()->size(); i++)
    {
      id = newDiagram->getAutho()->at(i);
      btn = m_btnGroup->button(id);
      btn->setEnabled(true);
    }
  }
  else
  {
    //no more diagram
  }
}

void CToolBox::onSelectButton(int id, CToolButton::STATE state)
{
  qDebug("onSelectButton %i %i", id, state);
  emit itemSelected(id, state);
  //TODO emit with class + state
}

void CToolBox::returnToSelectButton()
{
  //qDebug("returnToIdleButton");
  m_btnGroup->button(CToolItemModel::ITEM_SELECT)->setChecked(true);
}


CToolBox::~CToolBox()
{
  qDebug("~CToolBox");
}


