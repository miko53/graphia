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
#include "documentwriter.h"
#include <QDomDocument>
#include <QTextStream>
#include <QFile>
#include "graphia.h"
#include "diagrams/generic/xmlbasicitem.h"

CDocumentWriter::CDocumentWriter( CDocument* doc )
{
  m_doc = doc;
}

CDocumentWriter::~CDocumentWriter()
{

}

void CDocumentWriter::saveDocument( const QString& filename )
{
  QDomDocument doc("graphia-document");
 
  buildPreambule(doc);

  QDomElement root = doc.createElement("graphia-document");
  doc.appendChild(root);

  buildMetaInf(doc, root);
  buildDocument(doc, root);
  
  QFile file(filename);
  file.open(QIODevice::WriteOnly);
  QTextStream out(&file);
  doc.save(out, 2);
  file.close();
}


void CDocumentWriter::buildPreambule (QDomDocument& doc)
{
  QDomNode xmlPreambule = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"ISO-8859-1\"");
  doc.appendChild(xmlPreambule);
}


void CDocumentWriter::buildMetaInf(QDomDocument& doc, QDomElement& root)
{
  QDomElement metaInf = doc.createElement("meta-inf");
  root.appendChild(metaInf);

  insertVersion(doc, metaInf);

  QDomElement components = doc.createElement("components");
  metaInf.appendChild(components);

  insertListOfComponents(doc, components);

  QDomElement statistic = doc.createElement("stat");
  metaInf.appendChild(statistic); 
}

void CDocumentWriter::insertVersion ( QDomDocument& doc, QDomElement& metaInf )
{
  QDomElement docVersion = doc.createElement("doc-version");
  metaInf.appendChild(docVersion);
  QDomText docNoVersion = doc.createTextNode("1.0.0");
  docVersion.appendChild(docNoVersion);
}

void CDocumentWriter::insertListOfComponents ( QDomDocument& doc, QDomElement& components )
{
  CConfiguration* config = getApp()->getCfg();
  QDomElement component = doc.createElement("component");
  components.appendChild(component);
 
 int nbDiagram;
 nbDiagram = config->getListDiagramCount();
 
 for(int i = 0; i < nbDiagram; i++)
 {
    QDomElement name = doc.createElement("name");
    component.appendChild(name);
    QDomText flowName = doc.createTextNode(config->getDiagramNameAt(i));
    name.appendChild(flowName);
    QDomElement version = doc.createElement("version");
    component.appendChild(version); 
    QDomText flowVersion = doc.createTextNode(config->getDiagramVersionAt(i));
    version.appendChild(flowVersion);   
 }
}

void CDocumentWriter::buildDocument ( QDomDocument& doc, QDomElement& root )
{

  buildDefaultConfiguration(doc, root);
  
  QDomElement diagrams = doc.createElement("document");
  root.appendChild(diagrams);
  
  if (m_doc->getDocumentType() == FLOW_PROCESS)
  {
    diagrams.setAttribute("type", "flow-process");
    insertFlowType(doc, diagrams);
    insertFlowData(doc, diagrams);
    insertFlowModel(doc, diagrams);
    insertFlowDiagram(doc, diagrams);
  }
  else
  {
    //not another type for now, so it is a bug !
    Q_ASSERT(false);
  }
}

void CDocumentWriter::buildDefaultConfiguration ( QDomDocument& doc, QDomElement& root)
{
  CGraphicsConfiguration* config = getApp()->getGraphicsConfig();

  QDomElement default_configuration = doc.createElement("default-config");
  root.appendChild(default_configuration);
  
  QDomElement scene = doc.createElement("scene");
  default_configuration.appendChild(scene);
  CXmlBasicItem::insertQRectF(doc, scene, config->sceneRect());
  
  QDomElement circleRadius = doc.createElement("circleRadius");
  default_configuration.appendChild(circleRadius);
  CXmlBasicItem::insertQReal(doc, circleRadius, config->circleRadius());

  QDomElement arrowAngle = doc.createElement("arrowAngle");
  default_configuration.appendChild(arrowAngle);
  CXmlBasicItem::insertInt(doc, arrowAngle, (int)(config->arrowAngle()*180/3.141569));  
  
  QDomElement arrowLength = doc.createElement("arrowLength");
  default_configuration.appendChild(arrowLength);
  CXmlBasicItem::insertQReal(doc, arrowLength, config->arrowLength());

  QDomElement anchorSize = doc.createElement("anchorSize");
  default_configuration.appendChild(anchorSize);
  CXmlBasicItem::insertQReal(doc, anchorSize, config->anchorSize());
  
  QDomElement storeHeight = doc.createElement("storeHeight");
  default_configuration.appendChild(storeHeight);
  CXmlBasicItem::insertQReal(doc, storeHeight, config->storeHeight());

  QDomElement storeLength = doc.createElement("storeLength");
  default_configuration.appendChild(storeLength);
  CXmlBasicItem::insertQReal(doc, storeLength, config->storeLength());
  
  QDomElement alignmentH = doc.createElement("alignmentH");
  default_configuration.appendChild(alignmentH);
  CXmlBasicItem::insertHexaNumber(doc, alignmentH, config->alignmentH());
  
  QDomElement alignmentV = doc.createElement("alignmentV");
  default_configuration.appendChild(alignmentV);
  CXmlBasicItem::insertHexaNumber(doc, alignmentV, config->alignmentV());
  
  QDomElement displayLabel = doc.createElement("displayLabel");
  default_configuration.appendChild(displayLabel);
  CXmlBasicItem::insertInt(doc, displayLabel, config->displayLabel());  
  
  QDomElement font = doc.createElement("font");
  default_configuration.appendChild(font);
  CXmlBasicItem::insertQFont(doc, font, config->font());
  
  QDomElement fontColor = doc.createElement("fontColor");
  default_configuration.appendChild(fontColor);
  CXmlBasicItem::insertQColor(doc, fontColor, config->fontColor());
  
  QDomElement lineColor = doc.createElement("lineColor");
  default_configuration.appendChild(lineColor);
  CXmlBasicItem::insertQColor(doc, lineColor, config->lineColor());
  
  QDomElement backgroundColor = doc.createElement("backgroundColor");
  default_configuration.appendChild(backgroundColor);
  CXmlBasicItem::insertQColor(doc, backgroundColor, config->background());
  
}

void CDocumentWriter::insertFlowType( QDomDocument& doc, QDomElement& diagrams)
{
  CTypeFactory* typeFactory;
  typeFactory = m_doc->getItemFactory()->getTypeFactory();

  QDomElement flowTypes = doc.createElement("flow-types");
  diagrams.appendChild(flowTypes);
  
  for(int i=0; i< typeFactory->getNbOfType(); i++)
  {
    CClass* currentClass = typeFactory->getTypeAt(i);
    QDomElement flowType = doc.createElement("flow-type");
    flowTypes.appendChild(flowType);
   
    QDomElement id = doc.createElement("id");
    QDomText idValue = doc.createTextNode(QString::number(currentClass->getId()));
    flowType.appendChild(id);
    id.appendChild(idValue);

    QDomElement name = doc.createElement("name");
    QDomText className = doc.createTextNode(currentClass->getName());
    flowType.appendChild(name);
    name.appendChild(className);
    
    QDomElement def = doc.createElement("definition");
    QDomText classDef = doc.createTextNode(currentClass->getDefinition());
    flowType.appendChild(def);
    def.appendChild(classDef);
    
    QDomElement type = doc.createElement("type");
    QDomText typeValue = doc.createTextNode(QString::number(currentClass->getTypeOfClass()));
    flowType.appendChild(type);
    type.appendChild(typeValue);
    
    QDomElement readOnly = doc.createElement("read-only");
    QDomText readOnlyValue = doc.createTextNode(QString::number(currentClass->isReadOnly()));
    flowType.appendChild(readOnly);
    readOnly.appendChild(readOnlyValue);    

    QDomElement values = doc.createElement("values");
    flowType.appendChild(values);
   
    
    switch(currentClass->getTypeOfClass())
    {
      case CClass::INVALID:
      case CClass::VOID:
        break;
      
      case CClass::ENUMERATION:
      {
        classProperties p = currentClass->getProperties();
        foreach(QString s, *(p.enumerationList))
        {
          QDomElement enumeration = doc.createElement("string");
          QDomText enumStr = doc.createTextNode(s);
          enumeration.appendChild(enumStr);
          values.appendChild(enumeration);
        }
      }
        break;
        
      case CClass::NUMBER:
      case CClass::CHAR:
      case CClass::FLOAT:
      {
        classProperties p = currentClass->getProperties();
        //TODO va falloir ameliorer un peu tout cela !
        foreach(Range r, *(p.rangeList))
        {
          QDomElement range = doc.createElement("range");
          switch (r.begin.type())
          {
            case QVariant::Int:
              range.setAttribute("begin", QString::number(r.begin.toInt()));
              break;
            case QVariant::UInt:
              range.setAttribute("begin", QString::number(r.begin.toUInt()));
              break;
            case QVariant::LongLong:
              range.setAttribute("begin", QString::number(r.begin.toLongLong()));
              break;              
            case QVariant::ULongLong:
              range.setAttribute("begin", QString::number(r.begin.toULongLong()));
              break;              
            case QVariant::Double:
              range.setAttribute("begin", QString::number(r.begin.toDouble()));              
              break;
            default:
              qDebug("type of qvariant(begin) = %i", r.begin.type());
              break;
          }
          range.setAttribute("begin-name", r.beginName);
          switch (r.end.type())
          {
            case QVariant::Int:
              range.setAttribute("end", QString::number(r.end.toInt()));
              break;
            case QVariant::UInt:
              range.setAttribute("end", QString::number(r.end.toUInt()));
              break;
            case QVariant::LongLong:
              range.setAttribute("end", QString::number(r.end.toLongLong()));
              break;              
            case QVariant::ULongLong:
              range.setAttribute("end", QString::number(r.end.toULongLong()));
              break;              
            case QVariant::Double:
              range.setAttribute("end", QString::number(r.end.toDouble()));
              break;
            default:
              qDebug("type of qvariant(end) = %i", r.end.type());
              break;
          }        
          range.setAttribute("end-name", r.endName);
          values.appendChild(range);
        }
      }
        break;
        
      case CClass::STRING:
      {
        QDomElement size = doc.createElement("size");
        QDomText sizeValue = doc.createTextNode(QString::number(currentClass->getProperties().stringSize));
        size.appendChild(sizeValue);
        values.appendChild(size);
      }
        break;
        
      case CClass::AGGREGATION:
      {
       classProperties p = currentClass->getProperties();
        foreach(Structure s, *(p.aggregationList))
        {
          
          QDomElement structure = doc.createElement("struct");
          structure.setAttribute("name", s.name);
          structure.setAttribute("type", QString::number(s.pClass->getId()));
          values.appendChild(structure);
        }
      }
        break;
        
      default:
        qDebug("type %i not managed", currentClass->getTypeOfClass());
        break;
    }
  }
}

void CDocumentWriter::insertFlowData ( QDomDocument& doc, QDomElement& diagrams )
{
  CDataFactory* dataFactory;
  dataFactory = m_doc->getItemFactory()->getDataFactory();

  QDomElement flowDatas = doc.createElement("flow-datas");
  diagrams.appendChild(flowDatas);
  
  for(int i=0; i< dataFactory->getCount(); i++)
  {
    CData* data;
    data = dataFactory->getDataAt(i);
    QDomElement flowData = doc.createElement("flow-data");
    flowDatas.appendChild(flowData);
    
    QDomElement idElem = doc.createElement("id");
    QDomText idValue = doc.createTextNode(QString::number(data->getID()));
    idElem.appendChild(idValue);
    flowData.appendChild(idElem);
    
    QDomElement name = doc.createElement("name");
    QDomText namestr = doc.createTextNode(data->getName());
    name.appendChild(namestr);
    flowData.appendChild(name);
    
    QDomElement classElem = doc.createElement("class");
    QDomText classValue = doc.createTextNode(QString::number(data->getClass()->getId()));
    classElem.appendChild(classValue);
    flowData.appendChild(classElem);
    
    QDomElement typeElem = doc.createElement("flow-type");
    QDomText typeValue = doc.createTextNode(QString::number(data->getDataType()));
    typeElem.appendChild(typeValue);
    flowData.appendChild(typeElem);
  }
}

void CDocumentWriter::insertFlowModel ( QDomDocument& doc, QDomElement& diagrams )
{
  CProcessImplFactory* processFactory;
  
  processFactory = m_doc->getItemFactory()->getProcessFactory();
  
  QDomElement flowModel = doc.createElement("flow-model");
  diagrams.appendChild(flowModel);
  
  QDomElement implementationList = doc.createElement("implementation-process-list");
  flowModel.appendChild(implementationList);

  for(int i = 0; i < processFactory->getCount(); i++)
  {
    CProcessImplementation* processCurrent = processFactory->getAt(i);
    QDomElement process = doc.createElement("implementation-process");
    implementationList.appendChild(process);
    
    QDomElement idElem = doc.createElement("id");
    QDomText idValue = doc.createTextNode(QString::number(processCurrent->getID()));
    idElem.appendChild(idValue);
    process.appendChild(idElem);
    
    QDomElement name = doc.createElement("name");
    QDomText namestr = doc.createTextNode(processCurrent->getName());
    name.appendChild(namestr);
    process.appendChild(name);
    
    //list of flow
    insertFlowOfImplementedProcess(doc, process, processCurrent);
    //list of process instance
    insertProcessInstanceOfImplementedProcess(doc, process, processCurrent);
    //list of store
    insertStoreOfImplementedProcess(doc, process, processCurrent);
    //list of input
    insertInputOfImplementedProcess(doc, process, processCurrent);
    //list of output
    insertOutputOfImplementedProcess(doc, process, processCurrent);
  }
  
  QDomElement contextList = doc.createElement("context-process-list");
  flowModel.appendChild(contextList);
  CProcessContextFactory* contextProcessFactory;
  
  contextProcessFactory = m_doc->getItemFactory()->getContexProcessFactory();
  
  for(int i = 0; i < contextProcessFactory->getCount(); i++)
  {
    CProcessContext* processCtxCurrent = contextProcessFactory->getAt(i);
    
    QDomElement process = doc.createElement("context-process");
    contextList.appendChild(process);
    
    QDomElement idElem = doc.createElement("id");
    QDomText idValue = doc.createTextNode(QString::number(processCtxCurrent->getID()));
    idElem.appendChild(idValue);
    process.appendChild(idElem);
    
    QDomElement name = doc.createElement("name");
    QDomText namestr = doc.createTextNode(processCtxCurrent->getName());
    name.appendChild(namestr);
    process.appendChild(name);
    
    QDomElement hasChanged = doc.createElement("hasChanged");
    QDomText hasChangedValue = doc.createTextNode(QString::number(processCtxCurrent->nameHasChanged()));
    hasChanged.appendChild(hasChangedValue);
    process.appendChild(hasChanged);
    
    insertFlowOfContextProcess(doc, process, processCtxCurrent);
    insertStoreOfContextProcess(doc,process, processCtxCurrent);
    insertTerminatorOrContextProcess(doc, process, processCtxCurrent);
    
    insertProcessInstanceOfContextProcess(doc, process, processCtxCurrent);
    
/*    
  CProcessInstance* m_instanceOfContext;
*/   
  }
  
}


void CDocumentWriter::insertFlowOfImplementedProcess ( QDomDocument& doc, QDomElement& process, CProcessImplementation* processCurrent )
{
  QDomElement flowList = doc.createElement("flow-list");

  process.appendChild(flowList);
  
  for(int i = 0; i< processCurrent->getFLowcount(); i++)
  {
    CFlow* currentFlow = processCurrent->getFlowAt(i);
    QDomElement flow = doc.createElement("flow");
    flowList.appendChild(flow);
    
    QDomElement idElem = doc.createElement("id");
    QDomText idValue = doc.createTextNode(QString::number(currentFlow->getID()));
    idElem.appendChild(idValue);
    flow.appendChild(idElem);
    
    QDomElement name = doc.createElement("name");
    QDomText namestr = doc.createTextNode(currentFlow->getName());
    name.appendChild(namestr);
    flow.appendChild(name);
    
    if (currentFlow->getDataFlow() != NULL)
    {
      CData* d = currentFlow->getDataFlow();
      QDomElement data = doc.createElement("data-id");
      QDomText dataID = doc.createTextNode(QString::number(d->getID()));
      data.appendChild(dataID);
      flow.appendChild(data);
    }
    
    if (currentFlow->getInput() != NULL)
    {
      CItem* cItem = currentFlow->getInput();
      QDomElement input = doc.createElement("input");
      input.setAttribute("type", cItem->type());
      input.setAttribute("id", cItem->getID());
      flow.appendChild(input);      
    }
    
    if (currentFlow->getOutput() != NULL)
    {
      CItem* cItem = currentFlow->getOutput();
      QDomElement output = doc.createElement("output");
      output.setAttribute("type", cItem->type());
      output.setAttribute("id", cItem->getID());
      flow.appendChild(output);      
    }
  }
}


void CDocumentWriter::insertProcessInstanceOfImplementedProcess ( QDomDocument& doc, QDomElement& process, CProcessImplementation* processCurrent )
{
  QDomElement processInstanceList = doc.createElement("process-instance-list");

  process.appendChild(processInstanceList);
  
  for(int i = 0; i < processCurrent->getProcessInstanceCount(); i++)
  {
    CProcessInstance* processInstance = processCurrent->getProcessInstanceAt(i);
    
    QDomElement p = doc.createElement("process");
    processInstanceList.appendChild(p);
    
    QDomElement idElem = doc.createElement("id");
    QDomText idValue = doc.createTextNode(QString::number(processInstance->getID()));
    idElem.appendChild(idValue);
    p.appendChild(idElem);
    
    QDomElement name = doc.createElement("name");
    QDomText namestr = doc.createTextNode(processInstance->getName());
    name.appendChild(namestr);
    p.appendChild(name);
    
    QDomElement hasChanged = doc.createElement("hasChanged");
    QDomText hasChangedValue = doc.createTextNode(QString::number(processInstance->nameHasChanged()));
    hasChanged.appendChild(hasChangedValue);
    p.appendChild(hasChanged);
    
    if (processInstance->getImplementedProcess() != NULL)
    {
      QDomElement implementedProcess = doc.createElement("implements");
      implementedProcess.setAttribute("id", processInstance->getImplementedProcess()->getID());
      p.appendChild(implementedProcess);      
    }
    else
    {
      //no tag insertion when doesn't exist
    }
    
    //list of inputs
    QDomElement inputList = doc.createElement("input-list");
    p.appendChild(inputList);
    for (int j = 0; j < processInstance->getInputCount(); j++)
    {
      CConnector* connector = processInstance->getInputConnectorAt(j);
      QDomElement input = doc.createElement("input");
      if (connector->m_input != NULL)
        input.setAttribute("input-id", connector->m_input->getID());
      if (connector->m_flow != NULL)
        input.setAttribute("flow-id", connector->m_flow->getID());
      inputList.appendChild(input);
    }
        
    //list of outputs
    QDomElement outputList = doc.createElement("output-list");
    p.appendChild(outputList);
    for (int j = 0; j < processInstance->getOutputCount(); j++)
    {
      CConnector* connector = processInstance->getOutputConnectorAt(j);
      QDomElement output = doc.createElement("output");
      if (connector->m_input != NULL)
        output.setAttribute("output-id", connector->m_input->getID());
      if (connector->m_flow != NULL)
        output.setAttribute("flow-id", connector->m_flow->getID());
      outputList.appendChild(output);
    }
    
  }
}


void CDocumentWriter::insertStoreOfImplementedProcess ( QDomDocument& doc, QDomElement& process, CProcessImplementation* processCurrent )
{
  QDomElement storeList = doc.createElement("store-list");
  process.appendChild(storeList);
  
  for(int i = 0; i < processCurrent->getStoreCount(); i++)
  {
    CStore* s = processCurrent->getStoreAt(i);
    
    QDomElement storeElem = doc.createElement("store");
    storeList.appendChild(storeElem);
    
    QDomElement idElem = doc.createElement("id");
    QDomText idValue = doc.createTextNode(QString::number(s->getID()));
    idElem.appendChild(idValue);
    storeElem.appendChild(idElem);
    
    QDomElement name = doc.createElement("name");
    QDomText namestr = doc.createTextNode(s->getName());
    name.appendChild(namestr);
    storeElem.appendChild(name);
    
    QDomElement hasChanged = doc.createElement("hasChanged");
    QDomText hasChangedValue = doc.createTextNode(QString::number(s->nameHasChanged()));
    hasChanged.appendChild(hasChangedValue);
    storeElem.appendChild(hasChanged);
    
    
   //list of inputs
    QDomElement inputList = doc.createElement("input-list");
    storeElem.appendChild(inputList);
    for(int j = 0; j < s->getInputCount(); j ++)
    {
      CFlow* f = s->getInputFlowAt(j);
      QDomElement input = doc.createElement("input");
      
      if (f != NULL)
      {
        inputList.appendChild(input);
        input.setAttribute("flow-id", f->getID());
      }
      else
        Q_ASSERT(f != NULL); //it is not possible to store null flow, probably a bug :-( !!
    }
    
    //list of outputs
    QDomElement outputList = doc.createElement("output-list");
    storeElem.appendChild(outputList);
    for(int j = 0; j < s->getOutputCount(); j ++)
    {
      CFlow* f = s->getOutputFlowAt(j);
      QDomElement output = doc.createElement("output");
      if (f != NULL)
      {
        outputList.appendChild(output);
        output.setAttribute("flow-id", f->getID());
      }
      else
        Q_ASSERT(f != NULL); //it is not possible to store null flow, probably a bug :-( !!
    }
  }
}


void CDocumentWriter::insertInputOfImplementedProcess ( QDomDocument& doc, QDomElement& process, CProcessImplementation* processCurrent )
{
  QDomElement inputList = doc.createElement("input-list");
  process.appendChild(inputList);
  
  for(int i = 0; i < processCurrent->getInputCount(); i++)
  {
    CInput* input = processCurrent->getInputAt(i);
    QDomElement inputElem = doc.createElement("input");
    inputList.appendChild(inputElem);
    
    QDomElement idElem = doc.createElement("id");
    QDomText idValue = doc.createTextNode(QString::number(input->getID()));
    idElem.appendChild(idValue);
    inputElem.appendChild(idElem);
    
    QDomElement name = doc.createElement("name");
    QDomText namestr = doc.createTextNode(input->getName());
    name.appendChild(namestr);
    inputElem.appendChild(name);
    
    if (input->getFlow() != NULL)
    {
      CFlow* f = input->getFlow();
      QDomElement flowID = doc.createElement("flow");
      QDomText flowIDValue = doc.createTextNode(QString::number(f->getID()));
      flowID.appendChild(flowIDValue);
      inputElem.appendChild(flowID);
    }
    
    if (input->getDataFlow() != NULL)
    {
      CData* d = input->getDataFlow();
      QDomElement dataID = doc.createElement("data");
      QDomText dataIDValue = doc.createTextNode(QString::number(d->getID()));
      dataID.appendChild(dataIDValue);
      inputElem.appendChild(dataID);      
    }
  }
}

void CDocumentWriter::insertOutputOfImplementedProcess ( QDomDocument& doc, QDomElement& process, CProcessImplementation* processCurrent )
{
  QDomElement outputList = doc.createElement("output-list");
  process.appendChild(outputList);
  
  for(int i = 0; i < processCurrent->getOutputCount(); i++)
  {
    COutput* output = processCurrent->getOutputAt(i);
    QDomElement outputElem = doc.createElement("output");
    outputList.appendChild(outputElem);
    
    QDomElement idElem = doc.createElement("id");
    QDomText idValue = doc.createTextNode(QString::number(output->getID()));
    idElem.appendChild(idValue);
    outputElem.appendChild(idElem);
    
    QDomElement name = doc.createElement("name");
    QDomText namestr = doc.createTextNode(output->getName());
    name.appendChild(namestr);
    outputElem.appendChild(name);  
    
    if (output->getFlow() != NULL)
    {
      CFlow* f = output->getFlow();
      QDomElement flowID = doc.createElement("flow");
      QDomText flowIDValue = doc.createTextNode(QString::number(f->getID()));
      flowID.appendChild(flowIDValue);
      outputElem.appendChild(flowID);      
    }
    
    if (output->getDataFlow() != NULL)
    {
      CData* d = output->getDataFlow();
      QDomElement dataID = doc.createElement("data");
      QDomText dataIDValue = doc.createTextNode(QString::number(d->getID()));
      dataID.appendChild(dataIDValue);
      outputElem.appendChild(dataID);        
    }    
  }  
}





void CDocumentWriter::insertFlowOfContextProcess ( QDomDocument& doc, QDomElement& process, CProcessContext* processCtxCurrent )
{
  QDomElement flowList = doc.createElement("flow-list");

  process.appendChild(flowList);
  
  for(int i = 0; i< processCtxCurrent->getFLowcount(); i++)
  {
    CFlow* currentFlow = processCtxCurrent->getFlowAt(i);
    QDomElement flow = doc.createElement("flow");
    flowList.appendChild(flow);
    
    QDomElement idElem = doc.createElement("id");
    QDomText idValue = doc.createTextNode(QString::number(currentFlow->getID()));
    idElem.appendChild(idValue);
    flow.appendChild(idElem);
    
    QDomElement name = doc.createElement("name");
    QDomText namestr = doc.createTextNode(currentFlow->getName());
    name.appendChild(namestr);
    flow.appendChild(name);
    
    if (currentFlow->getDataFlow() != NULL)
    {
      CData* d = currentFlow->getDataFlow();
      QDomElement data = doc.createElement("data-id");
      QDomText dataID = doc.createTextNode(QString::number(d->getID()));
      data.appendChild(dataID);
      flow.appendChild(data);
    }
    
    if (currentFlow->getInput() != NULL)
    {
      CItem* cItem = currentFlow->getInput();
      QDomElement input = doc.createElement("input");
      input.setAttribute("type", cItem->type());
      input.setAttribute("id", cItem->getID());
      flow.appendChild(input);      
    }
    
    if (currentFlow->getOutput() != NULL)
    {
      CItem* cItem = currentFlow->getOutput();
      QDomElement output = doc.createElement("output");
      output.setAttribute("type", cItem->type());
      output.setAttribute("id", cItem->getID());
      flow.appendChild(output);      
    }
  }
}


void CDocumentWriter::insertStoreOfContextProcess ( QDomDocument& doc, QDomElement& process, CProcessContext* processCtxCurrent )
{
  QDomElement storeList = doc.createElement("store-list");
  process.appendChild(storeList);
  
  for(int i = 0; i < processCtxCurrent->getStoreCount(); i++)
  {
    CStore* s = processCtxCurrent->getStoreAt(i);
    
    QDomElement storeElem = doc.createElement("store");
    storeList.appendChild(storeElem);
    
    QDomElement idElem = doc.createElement("id");
    QDomText idValue = doc.createTextNode(QString::number(s->getID()));
    idElem.appendChild(idValue);
    storeElem.appendChild(idElem);
    
    QDomElement name = doc.createElement("name");
    QDomText namestr = doc.createTextNode(s->getName());
    name.appendChild(namestr);
    storeElem.appendChild(name);
    
    QDomElement hasChanged = doc.createElement("hasChanged");
    QDomText hasChangedValue = doc.createTextNode(QString::number(s->nameHasChanged()));
    hasChanged.appendChild(hasChangedValue);
    storeElem.appendChild(hasChanged);
    
    
   //list of inputs
    QDomElement inputList = doc.createElement("input-list");
    storeElem.appendChild(inputList);
    for(int j = 0; j < s->getInputCount(); j ++)
    {
      CFlow* f = s->getInputFlowAt(j);
      QDomElement input = doc.createElement("input");
      
      if (f != NULL)
      {
        inputList.appendChild(input);
        input.setAttribute("flow-id", f->getID());
      }
      else
        Q_ASSERT(f != NULL); //it is not possible to store null flow, probably a bug :-( !!
    }
    
    //list of outputs
    QDomElement outputList = doc.createElement("output-list");
    storeElem.appendChild(outputList);
    for(int j = 0; j < s->getOutputCount(); j ++)
    {
      CFlow* f = s->getOutputFlowAt(j);
      QDomElement output = doc.createElement("output");
      if (f != NULL)
      {
        outputList.appendChild(output);
        output.setAttribute("flow-id", f->getID());
      }
      else
        Q_ASSERT(f != NULL); //it is not possible to store null flow, probably a bug :-( !!
    }
  }
}


void CDocumentWriter::insertTerminatorOrContextProcess ( QDomDocument& doc, QDomElement& process, CProcessContext* processCtxCurrent )
{
  QDomElement terminatorList = doc.createElement("terminator-list");
  process.appendChild(terminatorList);
  
  for(int i = 0; i < processCtxCurrent->getTerminatorCount(); i++)
  {
    CTerminator* t = processCtxCurrent->getTerminatorAt(i);
    
    QDomElement terminatorElem = doc.createElement("terminator");
    terminatorList.appendChild(terminatorElem);
    
    QDomElement idElem = doc.createElement("id");
    QDomText idValue = doc.createTextNode(QString::number(t->getID()));
    idElem.appendChild(idValue);
    terminatorElem.appendChild(idElem);
    
    QDomElement name = doc.createElement("name");
    QDomText namestr = doc.createTextNode(t->getName());
    name.appendChild(namestr);
    terminatorElem.appendChild(name);
    
    QDomElement hasChanged = doc.createElement("hasChanged");
    QDomText hasChangedValue = doc.createTextNode(QString::number(t->nameHasChanged()));
    hasChanged.appendChild(hasChangedValue);
    terminatorElem.appendChild(hasChanged);
    
    QDomElement type = doc.createElement("type");
    QDomText typeValue = doc.createTextNode(QString::number(t->getType()));
    type.appendChild(typeValue);
    terminatorElem.appendChild(type);
    
    if (t->getInOrOutFlow() != NULL)
    {
      QDomElement flow = doc.createElement("flow");
      flow.setAttribute("id", t->getInOrOutFlow()->getID());
      terminatorElem.appendChild(flow);
    }
    
    if (t->getDataFlow() != NULL)
    {
      CData* d = t->getDataFlow();
      QDomElement data = doc.createElement("data-id");
      QDomText dataID = doc.createTextNode(QString::number(d->getID()));
      data.appendChild(dataID);
      terminatorElem.appendChild(data);
    }
  }
}

void CDocumentWriter::insertProcessInstanceOfContextProcess ( QDomDocument& doc, QDomElement& process, CProcessContext* processCtxCurrent )
{
    CProcessInstance* processInstance = processCtxCurrent->processInstance();
    
    QDomElement p = doc.createElement("process");
    process.appendChild(p);
    
    QDomElement idElem = doc.createElement("id");
    QDomText idValue = doc.createTextNode(QString::number(processInstance->getID()));
    idElem.appendChild(idValue);
    p.appendChild(idElem);
    
    QDomElement name = doc.createElement("name");
    QDomText namestr = doc.createTextNode(processInstance->getName());
    name.appendChild(namestr);
    p.appendChild(name);
    
    QDomElement hasChanged = doc.createElement("hasChanged");
    QDomText hasChangedValue = doc.createTextNode(QString::number(processInstance->nameHasChanged()));
    hasChanged.appendChild(hasChangedValue);
    p.appendChild(hasChanged);
    
    if (processInstance->getImplementedProcess() != NULL)
    {
      QDomElement implementedProcess = doc.createElement("implements");
      implementedProcess.setAttribute("id", processInstance->getImplementedProcess()->getID());
      p.appendChild(implementedProcess);      
    }
    else
    {
      //no tag insertion when doesn't exist
    }
    
    //list of inputs
    QDomElement inputList = doc.createElement("input-list");
    p.appendChild(inputList);
    for (int j = 0; j < processInstance->getInputCount(); j++)
    {
      CConnector* connector = processInstance->getInputConnectorAt(j);
      QDomElement input = doc.createElement("input");
      if (connector->m_input != NULL)
        input.setAttribute("input-id", connector->m_input->getID());
      if (connector->m_flow != NULL)
        input.setAttribute("flow-id", connector->m_flow->getID());
      inputList.appendChild(input);
    }
        
    //list of outputs
    QDomElement outputList = doc.createElement("output-list");
    p.appendChild(outputList);
    for (int j = 0; j < processInstance->getOutputCount(); j++)
    {
      CConnector* connector = processInstance->getOutputConnectorAt(j);
      QDomElement output = doc.createElement("output");
      if (connector->m_input != NULL)
        output.setAttribute("output-id", connector->m_input->getID());
      if (connector->m_flow != NULL)
        output.setAttribute("flow-id", connector->m_flow->getID());
      outputList.appendChild(output);
    }
}


void CDocumentWriter::insertFlowDiagram ( QDomDocument& doc, QDomElement& diagrams )
{
  QDomElement diagramList = doc.createElement("diagrams-list");
  diagrams.appendChild(diagramList);

  for(int i = 0; i< m_doc->getNbOfDiagram(); i++)
  {
    CGraphicsScene* currentScene = m_doc->getAt(i);
    QDomElement d = doc.createElement("diagram");
    diagramList.appendChild(d);
    
    QDomElement title = doc.createElement("title");
    QDomText titleS = doc.createTextNode(currentScene->getTitle());
    title.appendChild(titleS);
    d.appendChild(title);
    
    QDomElement type = doc.createElement("type");
    QDomText typeS = doc.createTextNode(QString::number(currentScene->getType()));
    type.appendChild(typeS);
    d.appendChild(type);

    QDomElement sceneRect = doc.createElement("scene");
    CXmlBasicItem::insertQRectF(doc, sceneRect, currentScene->sceneRect());
    d.appendChild(sceneRect);    
    
    //  QDomElement isDisplayed = diagram.firstChildElement("displayed");
    QDomElement displayed = doc.createElement("displayed");
    CXmlBasicItem::insertInt(doc, displayed, currentScene->isDisplayed());
    d.appendChild(displayed);
    
    if (currentScene->getItem() != NULL)
    {
      QDomElement item = doc.createElement("item");
      CItem* pItem = currentScene->getItem();
      item.setAttribute("type", pItem->type());
      item.setAttribute("id", pItem->getID());
      d.appendChild(item);
    }
    else
    {
      Q_ASSERT(false);
    }
    
    QList<QGraphicsItem*> gObjectList = currentScene->items(Qt::AscendingOrder);
    for(int j = 0; j < gObjectList.count(); j++)
    {
      QGraphicsItem* graphicsItems = gObjectList.at(j);
      if (graphicsItems->type() == CGraphicsItem::GraphicsItem)
      {        
        CGraphicsItem* gObject = dynamic_cast<CGraphicsItem*>(gObjectList.at(j));
        Q_ASSERT(gObject != NULL);
        gObject->insertItemIntoDomDocument(doc, d);
      }
    }
  }
}


