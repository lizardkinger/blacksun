#include "../include/SceneManager.h"
// for U/R - System:
#include "./../../core/include/UR_Manager.h"
#include "../include/Cmd_insertObject.h"
#include "../include/SceneUtils.h"

namespace BSScene
{
	
SceneManager* SceneManager::_instance = 0;

//Set version
BSPlgMgr::Version SceneManager::version(1,0,0);

SceneManager::SceneManager()
{
	init();
}

SceneManager::~SceneManager()
{
	release();
}

void SceneManager::init()
{
	m_description.clear();
	m_redrawNeeded = false;
	
	m_selectionMode = SELMODE_Mesh;
	m_actionMode = SCENEACTION_Selection; //SCENEACTION_None
	m_creationMode = CREATEMODE_Vertex;
	
	m_scene.reset();
}

void SceneManager::release()
{
	m_sbuffer.reset();
	m_scene.reset();
}

void SceneManager::resetMaterials()
{
	BSRenderer::MaterialManager* mm = BSRenderer::MaterialManager::getInstance();
	mm->removeAllMaterials();
}

SceneManager* SceneManager::getInstance()
{
	static SceneManagerGuard sceneguardian;
		
	if(_instance == NULL)
	{
		_instance = new SceneManager();
		_instance->init();
	}
	
	return _instance;
}

SceneObject* SceneManager::insertObject(SceneObject& o)
{
	QMutexLocker locker(&m_insertMutex);
	
	SceneObject* so = m_scene.addSceneObject(o);
	
	if(so->isKindOf(SCENETYPE_Mesh))
	{
		Mesh *m = dynamic_cast<Mesh*> (so);
		emit meshInserted( m->getName() );
	}
    
	//for U/R - System:
	BSCore::UR_Manager::getInstance()->push(new Cmd_insertObject(so));
    
    m_sbuffer.reset();
    m_sbuffer.addSceneObject(so,SCENETYPE_Select);
	
    checkForRedraw(true);
    
	return so;
}


void SceneManager::removeSelectedVertex(SelectedVertex& sv)
{
	Polygon *p = sv.getPolygon();
	Vertex *v = sv.getVertex();
	
	p->removeVertex(v);
}

void SceneManager::removeObject(SceneObject* o)
{
    if(o->isKindOf(SCENETYPE_Mesh))
    {
        Mesh *m = dynamic_cast<Mesh*> (o);
        emit meshRemoved( m->getName() );
    }
    
	if(o->isSelected())
		m_sbuffer.deselectSceneObject(o);

	QString mName;
	if(o->isKindOf(SCENETYPE_Mesh))
	{
		Mesh *m = dynamic_cast<Mesh*> (o);
		mName = m->getName();
	}
	
	m_scene.removeObject(o);
	emit meshRemoved( mName );
	
	checkForRedraw(true);
}

SelectionBuffer& SceneManager::getSelBuffer()
{
	return m_sbuffer;	
}

void SceneManager::interactiveVertCreation(const Vector &pos2D, SceneAxis /*axis*/)
{
	Vertex newVertex;
	newVertex.vPos.x = pos2D.x;
	newVertex.vPos.y = pos2D.y;
	newVertex.vPos.z = pos2D.z;
	
	SceneVertex sv;
	sv.setVertex(newVertex);
	
	insertObject(sv);
}

void SceneManager::interactivePolyCreation(const Vector &pos2D, bool /*done*/, SceneAxis /*axis*/)
{
	if(/*!done &&*/ m_vertexbuffer.size()<3)
	{
		LOG_Ok(QString("Creation working. Inserting new vertex.").toStdString());
		
		Vertex newVertex;
		
		//qDebug()<< "New polygon-vertex: " << pos2D.x << "," << pos2D.y << "," << pos2D.z;
		
   		newVertex.vPos.x = pos2D.x;
   		newVertex.vPos.y = pos2D.y;
   		newVertex.vPos.z = pos2D.z;
   		
   		bool alreadyIn = false;
   		for(vector<Vertex> ::iterator it = m_vertexbuffer.begin();it != m_vertexbuffer.end(); ++it)
   		{
   			if(	abs(newVertex.vPos.x - it->vPos.x) < 0.00001 &&
   				abs(newVertex.vPos.y - it->vPos.y) < 0.00001 &&
   				abs(newVertex.vPos.z - it->vPos.z) < 0.00001)
   				alreadyIn=true;
   		}
   		
   		if(m_vertexbuffer.size()==0)
   		{
   			newVertex.setTexCoord(-1,0.5,1.0);
   			
   		}
   		else if(m_vertexbuffer.size()==1)
   		{
   			newVertex.setTexCoord(-1,1.0,0.0);
   		}
   		else if(m_vertexbuffer.size()==2)
   		{
   			newVertex.setTexCoord(-1,0.0,0.0);
   		}
   		
   		if(!alreadyIn)
   			m_vertexbuffer.push_back(newVertex);
	}

	if(/*done &&*/ m_vertexbuffer.size()==3)
	{		
		LOG_Ok(QString("Creation done.").toStdString());
		Polygon newPoly;
		newPoly.setVertices(m_vertexbuffer);
			
		newPoly.triangulate();
		newPoly.calcNormals();
		newPoly.deSelect();
		
		insertObject(newPoly);

		m_vertexbuffer.clear();
	}

	m_redrawNeeded = true;
}


void SceneManager::renderCreationState(Renderer& r)
{
	Matrix matIdent;
	matIdent.identity();
	
	Aabb bound;
	vector<Vertex> vPoint;
	
	for(vector<Vertex> ::iterator it = m_vertexbuffer.begin();it != m_vertexbuffer.end(); ++it)
	{
		Vector p = it->vPos;
		vPoint.push_back(Vertex(Vector(p.x,p.y,p.z),	Vector( 0.0,  1.0,  0.0),0.0,0.0));
	}
	
	switch(vPoint.size())
	{
		case 1:	r.render(PRIM_Point, vPoint, 0, matIdent, bound);
				break;
		
		case 2: r.render(PRIM_Line, vPoint, 0, matIdent, bound);
				break;
	
		default: break;
	}
}

void SceneManager::reset()
{
		release();
		init();
		sceneChanged();
		emit meshChange();
}

void SceneManager::setDescription(const QString s)
{
	m_description = s;
}


QString SceneManager::getDescription() const
{
	return m_description;
}


vector<QString> SceneManager::getSceneobjectNames()
{
	vector<QString> nameList;
	
	vector<QString> temp = m_scene.getMeshNames();
	
	for(vector<QString>::iterator it = temp.begin();it != temp.end(); ++it)
		nameList.push_back( *it );
	
	return nameList;
}

bool SceneManager::saveScene(const QString name)
{
	QFile file(name);
	
	file.open(QIODevice::WriteOnly);
	if(!file.isOpen())
 		return false;

	QByteArray arrayToCompress;
 	QDataStream out(&arrayToCompress, QIODevice::ReadWrite);  
 	
 	out << (qint32)100;
 	out << (QString)m_description;
	
	m_scene.save(out);

	QByteArray arrayToSave = qCompress(arrayToCompress);
	
	QDataStream compressedFile(&file);
	compressedFile << (quint32)FILEID;
	compressedFile << (qint32)arrayToSave.size();
	//qDebug() << "Save: lengthOfCompressedData" << arrayToSave.size();
	
	compressedFile << arrayToSave;

	file.close();
	
	MaterialManager* mm = MaterialManager::getInstance();
	string mname(name.toStdString());
	mm->appendMaterialsToFile(mname);
	
	
	
	return true;
}

void SceneManager::changeTitle(const QString message)
{
	QString msg;
	
	if(message.length() == 0)
		msg = "";
	else
		msg = " - " + message;
		
	emit changeWindowTitle(msg);
}

bool SceneManager::loadScene(const QString name,const bool resetScene,const bool keepMaterial)
{
	if(resetScene)
		reset();
	
	QFile file(name);
	
	file.open(QIODevice::ReadOnly);
	if(!file.isOpen())
 		return false;	
 	
 	QDataStream in(&file);
 
	quint32 magic;
	in >> magic;
	if (magic != FILEID)
	{
		QMessageBox::information(NULL, "Error during loading", "Selected file is no blacksun source file");
    	return false;
	}
	
	qint32 lengthOfCompressedData;
	in >> lengthOfCompressedData;
	//qDebug() << "Load: lengthOfCompressedData" << lengthOfCompressedData;

	QByteArray compressedData;
	in >> compressedData;

	QByteArray uncompressedData;
	uncompressedData = qUncompress(compressedData);

	QDataStream uncompressedIn(&uncompressedData, QIODevice::ReadOnly);

 	int versionNumber;
 	uncompressedIn >> versionNumber;
 	if (versionNumber < 100)
 	{
    	QMessageBox::information(NULL, "Error during loading", "Selected file was saved by a earlier version of blacksun");
     	return false;
 	}
 	
 	QString desc;
 	uncompressedIn >> desc;
 	setDescription(desc);
 	
 	m_scene.load(uncompressedIn); 
 	int pos = file.pos(); 
 	file.close();

	if(!keepMaterial)
	{
 		MaterialManager* mm = MaterialManager::getInstance();
		string mname(name.toStdString());
		mm->loadMaterialsFromFilePos(mname,pos );
	}
 			
	emit meshChange(); 			
	checkForRedraw(true);
	 			
	return true;
}



void SceneManager::rotateSelection(const double amount,const  SceneAxis axis)
{
	m_sbuffer.rotate(axis,amount);	
}

void SceneManager::scaleSelection(const double percentageX,const double percentageY,const double percentageZ)
{
	m_sbuffer.scale(percentageX,percentageY,percentageZ);
}

void SceneManager::moveSelection(const QPointF& source,const  QPointF& target,const  SceneAxis axis)
{
	m_sbuffer.move(SceneAction2D::getGUIDistance(source,target,axis));
}

void SceneManager::action2D( const SceneAction2D &sa)
{
	Vector vecNew;
				
	switch(m_actionMode)
	{
		case SCENEACTION_Move:
			moveSelection(sa.topleft, sa.bottomright,  sa.axis);
			break;
			
		case SCENEACTION_Selection:
			selection2D(sa.topleft,sa.bottomright,sa.axis,sa.selType);
			break;
					
		case SCENEACTION_Scale:
			scaleSelection(sa.scaleValueX,sa.scaleValueY,sa.scaleValueZ);
			break;		
			
		case SCENEACTION_Rotate:
			rotateSelection(sa.rotateValue, sa.axis);
			break;					
		
		case SCENEACTION_Creation:

			switch(sa.axis)
			{
				case AXIS_x:	
					vecNew = Vector(0,sa.topleft.y(),sa.topleft.x());
					break;
				
				case AXIS_y:	
					vecNew = Vector(sa.topleft.x(),0,-sa.topleft.y()) ;
					break;		
				
				case AXIS_z:
					vecNew = Vector(sa.topleft.x(),sa.topleft.y(),0) ;
					break;		
			}
			
			switch(m_creationMode)
			{
				case CREATEMODE_Polygon:
				{
					bool done;
					if(sa.mouseButton == Qt::RightButton)
						done = true;
					else
						done = false;
									
					interactivePolyCreation(vecNew,done,sa.axis);
				}
				break;
				
				case CREATEMODE_Vertex:
				{
					interactiveVertCreation(vecNew,sa.axis);
				}
				break;
				
				default: break;
			}
			

			break;
	
		case SCENEACTION_None:
			break;
			
		default:
			break;
	}
	
	checkForRedraw(true);
}
  



void SceneManager::selection2D(const QPointF& min, const QPointF& max,const SceneAxis axis, const SceneAction2D_SelectionType selType)
{
	if(selType == SCENETYPE_Select)
		m_sbuffer.reset();
	
	if(selType == SCENETYPE_AddToSelection)
		qDebug() << "SceneManager::selection2D - SCENETYPE_AddToSelection";
	
	if(selType == SCENETYPE_Select)
		qDebug() << "SceneManager::selection2D - SCENETYPE_Select";
		
	if(selType == SCENETYPE_RemoveFromSelection)
		qDebug() << "SceneManager::selection2D - SCENETYPE_RemoveFromSelection";
	
	Vector vecMin,vecMax;
	
	float fMin = -999999;
	float fMax = +999999;
	
	switch(axis)
	{
		case AXIS_x:	// Side
			vecMin = Vector(fMin,min.y(),min.x());
			vecMax = Vector(fMax,max.y(),max.x());
			break;
		
		case AXIS_y:	// Top
			vecMin = Vector(min.x(),fMin,-min.y());
			vecMax = Vector(max.x(),fMax,-max.y());
			break;		
		
		case AXIS_z:	// Front
			vecMin = Vector(min.x(),min.y(),fMin);
			vecMax = Vector(max.x(),max.y(),fMax);
			break;		
	}
	
	SceneUtils::getMinMax(vecMin,vecMax);
		
	Aabb section;
	section.setMinimum(vecMin);
	section.setMaximum(vecMax);
	section.setCentre((vecMax + vecMin)/2.0);
	
	m_scene.selectIntersecting(m_selectionMode,m_sbuffer,section,axis,selType);
	
	m_redrawNeeded = true;
}

void SceneManager::checkForRedraw(const bool redraw,const bool justSBuffer)
{
	if(redraw || m_redrawNeeded)
	{
		m_sbuffer.adjustSelectionBoundaries();
		
		emit sceneChanged();
	}
		
	m_redrawNeeded = false;
}

void SceneManager::renderScene(Renderer& r)
{
	m_scene.render(r);
	
	renderCreationState(r);
	
	m_sbuffer.render(r);
	
	r.flush();
	
	m_redrawNeeded = false;
}

void SceneManager::notifyContentChanged()
{
	notifyContentChanged(SCENETYPE_All,false);
}

void SceneManager::notifyContentChanged(const SceneObject *so)
{
	notifyContentChanged(so->getType(),so->isSelected());
}

void SceneManager::notifyContentChanged(const SceneObjectType sot,const bool justSBuffer)
{
	switch(sot)
	{
		case SCENETYPE_Mesh:
		{
			emit meshChange(); 	
		}
		break;	
			 
		case SCENETYPE_Vertex:
		case SCENETYPE_Polygon:
		{
		 	
		}
		break;	
		
		default: break;
		
	}
	
	checkForRedraw(true,justSBuffer);
	
}



}
