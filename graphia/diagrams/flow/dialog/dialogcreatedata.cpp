/*
graphia - little tool for building designer diagram
Copyright (C) 2012  Mickael Sergent

This program is free software:
    you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
         but WITHOUT ANY WARRANTY;
without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "dialogcreatedata.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>

CDialogCreateData::CDialogCreateData( CDataFactory* datafactory, CTypeFactory* typefactory, QWidget* parent, Qt::WindowFlags f ) : QDialog(parent,f)
{
  m_dataFactory = datafactory;
  m_typeFactory = typefactory;
  m_dataCreated = NULL;
  setWindowTitle(tr("graphia - create a data flow"));  
  buildDialog();
}


void CDialogCreateData::buildDialog()
{
  QGridLayout* g1 = new QGridLayout();
  
  QLabel* labelName = new QLabel(tr("flow name :"));
  m_flowName = new QLineEdit();
  m_flowName->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  connect(m_flowName, SIGNAL(editingFinished()), this, SLOT(onEndNameEditing())); 

  m_labelNameValid = new QLabel();

  QLabel* labelFlowType = new QLabel(tr("flow type :"));
  m_flowType = new QComboBox();
  m_flowType->setEditable(false);
  m_flowType->insertItem(0, tr("data flow"), CData::DATA);
  m_flowType->insertItem(1, tr("control flow"), CData::CONTROL);
  m_flowType->insertItem(2, tr("contineous flow"), CData::CONTINUEOUS);
  
  m_flowType->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  
  QLabel* labelDataType = new QLabel(tr("data type :"));
  m_dataType = new QComboBox();
  m_dataType->insertItems(0, *m_typeFactory->getListOfName());
  QPushButton* btnAddDataType = new QPushButton(QIcon(":/create.png"), tr("Add..."));
  
  g1->addWidget(labelName, 0, 0);
  g1->addWidget(m_flowName, 0, 1);
  g1->addWidget(m_labelNameValid, 0, 2);
  g1->addWidget(labelFlowType, 1, 0);
  g1->addWidget(m_flowType, 1, 1);
  g1->addWidget(labelDataType, 2, 0);
  g1->addWidget(m_dataType, 2, 1);
  g1->addWidget(btnAddDataType, 2, 2);
  
  
  QDialogButtonBox* b1 = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal);

  connect(b1, SIGNAL(accepted()), this, SLOT(accept()));
  connect(b1, SIGNAL(rejected()), this, SLOT(reject()));
  
  QVBoxLayout* v1 = new QVBoxLayout();
  v1->addLayout(g1);
  v1->addSpacing(20);
  v1->addWidget(b1);
  
  setLayout(v1); 
}

void CDialogCreateData::onEndNameEditing()
{
  //qDebug("CDialogCreateData::onEndNameEditing");
  QStringList* dataStrList;
  dataStrList = m_dataFactory->getListOfName();
  
  if (dataStrList->contains(m_flowName->text()))
  {
    m_labelNameValid->setText(tr("<font color=\"red\">name not valid</font>"));
  }
  else
  {
    m_labelNameValid->setText(tr("<font color=\"green\">name valid</font>"));    
  }
}


void CDialogCreateData::accept()
{
  //qDebug("CDialogCreateData::accept");
  CData* data = NULL;
  CClass* cl = NULL;
  cl = m_typeFactory->getTypeAt(m_dataType->currentIndex());
  Q_ASSERT(cl != NULL);
  
  data = m_dataFactory->createNewData(m_flowName->text(),
                                      cl,
                                      (CData::dataType) m_flowType->currentIndex());
  qDebug("creation of new data flow \"%s\"(type = %p --> \"%s\", %i)",
         m_flowName->text().toLocal8Bit().data(),
         cl, cl->getName().toLocal8Bit().data(),
         m_flowType->currentIndex());
         
  if (data)
  {
    m_dataCreated = data;
    QDialog::accept();
  }
}



CDialogCreateData::~CDialogCreateData()
{

}

