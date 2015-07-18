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

#include <QMessageBox>
#include "document.h"
#include "diagrams/flow/model/processimplementation.h"
#include "diagrams/flow/model/processinstance.h"
#include "diagrams/flow/model/processcontext.h"
#include "graphia.h"
#include "diagrams/flow/graphics/graphicscircle.h"
#include "diagrams/flow/storage/documentwriter.h"
#include "diagrams/flow/storage/documentreader.h"


CDocument::CDocument()
{
  m_type = FLOW_PROCESS;
  m_isModified = false;
  m_itembuilder = NULL;
}

CDocument::~CDocument()
{

}

void CDocument::createNewDocument()
{
  cleanDocument();
  emit newDocumentCreated();
}

void CDocument::cleanDocument()
{
  if (m_itembuilder != NULL)
    delete m_itembuilder;

  m_itembuilder = new CItemFactory();
  
  foreach(CGraphicsScene* scene, m_diagramList)
  {
    removeDiagram(scene);
  }
  m_diagramList.clear();  
  emitDocumentIsModified(false);
}

void CDocument::emitDocumentIsModified ( bool modifiedValue )
{
  //qDebug("emitDocumentIsModified");
  if (((modifiedValue == false) && (m_isModified == true)) ||
      ((modifiedValue == true) && (m_isModified == false)))
  {
    m_isModified = modifiedValue;
    emit documentModified(m_isModified);
  }
}


bool CDocument::save( const QString& filename )
{
  CDocumentWriter writer(this);
  writer.saveDocument(filename);
  emitDocumentIsModified(false);
  return true;//TODO
}


bool CDocument::read ( const QString& filename )
{
  CDocumentReader reader(this);
  bool result;
  result = reader.readFile(filename);
  
  if (result)
  {
    cleanDocument();
    foreach(CGraphicsScene* s, m_diagramList)
      removeDiagram(s);
    m_diagramList.clear();
    m_diagramList = *reader.getDiagramList(); //TODO FIXME memory leak ?
    delete m_itembuilder;
    m_itembuilder = reader.getItemBuilder();
    //reader.getReadConfig();
    
    displayTheDiagrams();    
  }
  return result;
}

void CDocument::displayTheDiagrams()
{
  foreach(CGraphicsScene* scene, m_diagramList)
  {
    if (scene->isDisplayed())
    {
      scene->setDisplayState(false);
      emit displayNewDiagram(scene);
    }
  }
}


bool CDocument::addNewDiagram(int type, const QString& diagramName, const QString& processName)
{
  CGraphicsScene* scene;
  struct existingDiagram chkExistingDiagram;
  
  scene = NULL;
  chkExistingDiagram = checkExistingDiagram(processName);

  switch (type)
  {
    case 0: //TODO : do a better type management instead of number, peut etre deplacer cela dans le model CToolItemModel...
      switch (chkExistingDiagram.diagram)
      {
        case 0: //doesn't exist create a new one 
          scene = createContextDiagram(diagramName, processName, NULL);  
          break;
          
        case 1: //a diagram already exists, refuse the creation
        case 3:
          QMessageBox::warning(NULL, tr("graphia - unable to create diagram"), 
                                tr("It is not possible to create a diagram with this name of process, because a diagram already exist with for this process.\n"
                                "Try to display it.")
                                );  
          break;
          
        case 2:  //ony a process diagram exist, append a new diagram with the selected item
          scene = createContextDiagram(diagramName, processName, chkExistingDiagram.item);
          break;
          
        default:
          Q_ASSERT(false);
          break;
      }
      break;
      
    case 1: 
      switch (chkExistingDiagram.diagram)
      {
        case 0: //doesn't exist, create a new one
          scene = createImplementationProcessDiagram(diagramName, processName, NULL);
          break;
          
        case 1: //context diagram exist and only context diagram, append a new diagram to this process
          scene = createImplementationProcessDiagram(diagramName, processName, chkExistingDiagram.item);
          break;
        
        case 2: // already a diagram of same type exist, refuse the creation
        case 3: //all type of diagramm already exist, refuse creation
          QMessageBox::warning(NULL, tr("graphia - unable to create diagram"), 
                                tr("It is not possible to create a diagram with this name, because a diagram already exist with this name.\n"
                                "Try to display it.")
                                );
          break;
          
        default:
          Q_ASSERT(false);
          break;
      }
      break;
      
    default:
      Q_ASSERT(false);
      break;
  }
  
  if (scene != NULL)
  {
    m_diagramList.append(scene);
    emit displayNewDiagram(scene);
    emitDocumentIsModified(true);
  }
  

  return (scene != NULL);
}


CDocument::existingDiagram CDocument::checkExistingDiagram(const QString& name)
{
  struct existingDiagram result;

  result.diagram = 0;
  result.item = NULL;
  
  
  foreach(CGraphicsScene* scene, m_diagramList)
  {
    switch (scene->getType())
    {        
      case 0:
        if (scene->getItem()->getName() == name)
        {
          result.diagram |= 1;
          Q_ASSERT(scene->getItem()->type() == CProcessContext::CPROCESS_CONTEXT);
          result.item = dynamic_cast<CProcessContext*>(scene->getItem())->processInstance()->getImplementedProcess();
        }
        break;
        
      case 1:
        if (scene->getItem()->getName() == name)
        {
          result.diagram |= 2;
          result.item = scene->getItem();
        }
        break;
        
      default:
        Q_ASSERT(0);
        break;
    }
  }
  
  qDebug("found :result.item = %p, result.diagram = %i\n", result.item, result.diagram);
  return result;
}


CGraphicsScene* CDocument::createImplementationProcessDiagram(const QString& diagramName, const QString& processName, CItem* exitingItem)
{
  CGraphicsScene* scene;
  CProcessImplementation* process;
  
  if (exitingItem == NULL)
  {
    process = m_itembuilder->createNewImplementationProcess(processName);
  }
  else
  {
    Q_ASSERT(exitingItem->type() == CProcessImplementation::CPROCESS_IMPL);
    process = dynamic_cast<CProcessImplementation*>(exitingItem);
  }
    
  scene = new CGraphicsScene();
  scene->setTitle(diagramName);
  process->setScene(scene);
  scene->setItem(process);
  scene->setType(1);
  buildAuthorizationList(scene);  
  return scene;
}


CGraphicsScene* CDocument::createContextDiagram(const QString& diagramName, const QString& processName, CItem* exitingItem)
{
  CGraphicsScene* scene;
  CProcessContext* process;
  CProcessInstance* instanceOfContext;
  CProcessImplementation* linkedImplProcess;
  
  process = m_itembuilder->createNewContextProcess(processName);

  if (exitingItem == NULL)
  {
    linkedImplProcess = m_itembuilder->createNewImplementationProcess(processName);
  }
  else
  {
    Q_ASSERT(exitingItem->type() == CProcessImplementation::CPROCESS_IMPL);
    linkedImplProcess = dynamic_cast<CProcessImplementation*>(exitingItem);
  }
  
  instanceOfContext = new CProcessInstance();
  instanceOfContext->setImplementedProcess(linkedImplProcess);
  instanceOfContext->setName(processName);
  instanceOfContext->setParent(process);
  process->setProcessInstance(instanceOfContext);
  QPointF point(210., 120.);//TODO
  QGraphicsItem* graphItem = new CGraphicsCircle(point, instanceOfContext);
  dynamic_cast<CGraphicsItem*>(graphItem)->resetProperties(CGraphicsItem::IS_REMOVABLE);

  scene = new CGraphicsScene();
  scene->setTitle(diagramName);
  process->setScene(scene);
  scene->setItem(process);
  scene->setType(0);
  
  buildAuthorizationList(scene);
  scene->addItem(graphItem);

  return scene;
}

void CDocument::buildAuthorizationList ( CGraphicsScene* scene )
{
  Q_ASSERT(scene != 0);
  
  switch(scene->getType())
  {
    case 0:
      scene->appendAuthorization(CToolItemModel::ITEM_ARROW);
      scene->appendAuthorization(CToolItemModel::ITEM_STORE);
      //scene->appendAuthorization(CToolItemModel::ITEM_BUS);TODO
      //scene->appendAuthorization(CToolItemModel::ITEM_BUS_LINK);
      scene->appendAuthorization(CToolItemModel::ITEM_TERMINATOR);
      break;
      
    case 1:
      scene->appendAuthorization(CToolItemModel::ITEM_PROCESS);
      scene->appendAuthorization(CToolItemModel::ITEM_ARROW);
      scene->appendAuthorization(CToolItemModel::ITEM_INPUT);
      scene->appendAuthorization(CToolItemModel::ITEM_OUTPUT);
      //scene->appendAuthorization(CToolItemModel::ITEM_BUS);TODO
      //scene->appendAuthorization(CToolItemModel::ITEM_BUS_LINK);
      scene->appendAuthorization(CToolItemModel::ITEM_STORE);
      break;
  }
}


void CDocument::removeDiagram(CGraphicsScene* diagram)
{
  bool bRemoveDone;
  
  emit diagramToBeRemoved(diagram);
  
  bRemoveDone = m_diagramList.removeOne(diagram);
  Q_ASSERT(bRemoveDone == true);
  
  getApp()->getModel()->removeDiagramItem(diagram);
}



bool CDocument::createNewImplementationProcessDiagram ( const QString& name)
{
  CGraphicsScene* scene;
  bool bOk;
  scene = searchExistingDiagram(name);
  if (scene == NULL)
  {
    scene = createImplementationProcessDiagram(name, name, NULL);
    m_diagramList.append(scene);
    emit displayNewDiagram(scene);
    bOk = true;
  }
  else
  {
    Q_ASSERT(false);//not possible
    bOk = false;
  }
  
  return bOk;
}

CGraphicsScene* CDocument::searchExistingDiagram ( const QString& name)
{
  CItem* item = NULL;
  CGraphicsScene* sceneFound = NULL;
  
  foreach(CGraphicsScene* scene, m_diagramList)
  {
    item = scene->getItem();
    switch(item->type())
    {
      case CProcessImplementation::CPROCESS_IMPL:
        if (name == item->getName())
        {
            //found;
          sceneFound = scene;
          break;
        }
      break;
      
      case CProcessContext::CPROCESS_CONTEXT:
      default:
        //Do nothing
      break;
    }
  }
  
  return sceneFound;
}


bool CDocument::displayDiagram ( CGraphicsScene* scene )
{
  emit displayNewDiagram(scene);
  return true;
}
