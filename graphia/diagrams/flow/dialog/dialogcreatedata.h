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

#ifndef DIALOGCREATEDATA_H
#define DIALOGCREATEDATA_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>

#include "diagrams/flow/model/datafactory.h"
#include "diagrams/flow/model/typefactory.h"

class CDialogCreateData : public QDialog
{
  Q_OBJECT
public:
  explicit CDialogCreateData (CDataFactory* datafactory, CTypeFactory* typefactory, QWidget* parent = 0, Qt::WindowFlags f = 0 );    
  virtual ~CDialogCreateData();

  CData* getDataFlowCreated() { return m_dataCreated; }
  
private:
  void buildDialog();
  
protected:
  QLineEdit* m_flowName;
  QComboBox* m_flowType;
  QComboBox* m_dataType;
  QLabel* m_labelNameValid;

  CDataFactory* m_dataFactory;
  CTypeFactory* m_typeFactory;
  
  CData* m_dataCreated;
  
protected slots:
  virtual void accept();
  void onEndNameEditing();
};

#endif // DIALOGCREATEDATA_H
