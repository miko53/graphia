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

#include "dialogchoosedata.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QHeaderView>
#include <QDialogButtonBox>
#include "dialogcreatedata.h"

CDialogChooseData::CDialogChooseData(CDataFactory* dataFactory, CTypeFactory* typeFactory, QWidget* parent, Qt::WindowFlags f): QDialog(parent, f)
{
  setWindowTitle(tr("graphia - choose a data"));
  Q_ASSERT(dataFactory != NULL);
  Q_ASSERT(typeFactory != NULL);
  
  m_model = new CDataFlowModel(dataFactory);
  m_typeFactory = typeFactory;
  buildDialog();
}

void CDialogChooseData::buildDialog()
{
  QVBoxLayout* v1 = new QVBoxLayout();
  QGridLayout* g1 = new QGridLayout();

  QLabel* l1 = new QLabel(tr("Select the data flow from the current list"));
  QLabel* filter = new QLabel(tr("filter"));
  QLineEdit* edit = new QLineEdit();
  connect(edit, SIGNAL(textChanged(QString)), this, SLOT(changeRegExp(QString)));
  QPushButton* button = new QPushButton(QIcon(":/create.png"), tr("Add..."));
  connect(button, SIGNAL(pressed()), this, SLOT(onClickAddButton()));
  
  m_table = new QTableView();
  buildTableWidget(m_table);
  connect(m_table,SIGNAL(doubleClicked(QModelIndex)), this, SLOT(applyModification()));
  
  QDialogButtonBox* dlgButton = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal);
  dlgButton->setCenterButtons(false);
  
  connect(dlgButton, SIGNAL(accepted()), this, SLOT(applyModification()));
  connect(dlgButton, SIGNAL(rejected()), this, SLOT(reject()));
  
  v1->addWidget(l1);
  g1->addWidget(filter, 0, 0);
  g1->addWidget(edit, 0, 1);
  g1->addWidget(button, 0, 2);
  v1->addLayout(g1);
  v1->addWidget(m_table);
  v1->addWidget(dlgButton);
 
  setLayout(v1);
}

void CDialogChooseData::buildTableWidget(QTableView* table)
{
  table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  table->setMinimumWidth(300);
  table->setMinimumHeight(300);
  table->horizontalHeader()->setStretchLastSection(true);
  table->horizontalHeader()->show();
  table->verticalHeader()->hide();
  table->setSelectionBehavior(QAbstractItemView::SelectRows);
  table->setSelectionMode(QAbstractItemView::SingleSelection);
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  
  m_proxyModel = new QSortFilterProxyModel(this);
  m_proxyModel->setSourceModel(m_model);
  table->setModel(m_proxyModel);
}

void CDialogChooseData::changeRegExp(const QString& edit)
{
  //qDebug("regExp changed (%s)", edit.toLocal8Bit().data());
  QRegExp expression;
  expression.setPatternSyntax(QRegExp::Wildcard);
  expression.setPattern(edit);
  if (expression.isValid())
  {
    //qDebug("expression is valid");
    m_proxyModel->setFilterRegExp(expression);
  }
  else
  {
    //qDebug("expression is not valid");
  }
}


void CDialogChooseData::applyModification()
{
  //select current row is selected.
  //qDebug("applyModification");
  QItemSelectionModel *select = m_table->selectionModel();
  if (select->hasSelection())
  {
    //qDebug("data is selected");
    QModelIndex index = select->currentIndex();
    QVariant v = m_model->data(index, Qt::UserRole);
    m_dataTaken = (CData*) v.value<void *>();
    done(QDialog::Accepted);
  }
  else
  {
    //qDebug("data is NOT selected");
  }
}


void CDialogChooseData::onClickAddButton()
{
  CDialogCreateData dlgCreateData(m_model->getDataFactory(), m_typeFactory);
  dlgCreateData.exec();
  m_model->update();
}


CDialogChooseData::~CDialogChooseData()
{
  //qDebug("delete ~CDialogChooseData");
  if (m_model)
    delete m_model;
}


