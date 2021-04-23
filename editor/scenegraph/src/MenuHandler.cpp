#include "../include/MenuHandler.h"
#include "../include/SceneManager.h"
#include "../include/TextureOffRepDlg.h"
#include <QMessageBox>
#include <QInputDialog>
 
namespace BSScene
{

MenuHandler::MenuHandler() 
{
	m_file_savename.clear();
}	

MenuHandler::~MenuHandler()
{
	
}

void MenuHandler::saveSMState()
{
	SceneManager *sm = SceneManager::getInstance();
	
	m_save_ssm = sm->getSelectionMode();
	m_save_sam = sm->getActionMode();
	m_save_scm = sm->getCreationMode();
}

void MenuHandler::loadSMState()
{
	SceneManager *sm = SceneManager::getInstance();	

	sm->setActionMode(m_save_sam);
	sm->setSelectionMode(m_save_ssm);
	sm->setCreationMode(m_save_scm);
	sm->changeTitle("");
}

QString MenuHandler::convertFilename(const QString &fname)
{
	QString result;
	
	int lPos=0,wPos=0,pos=0;
	
	if((lPos = fname.lastIndexOf("/")) == -1 && (wPos = fname.lastIndexOf("\\")) == -1)
		return fname;
	else
	{
		pos = (lPos != -1 ? lPos : wPos);
		
		result = fname.right( fname.length() - pos - 1);
		return result;
	}
}

void MenuHandler::file_new()
{
	m_file_savename = "";
	m_savedMeshes.clear();
	m_savedPolys.clear();
	
	saveSMState();
	SceneManager::getInstance()->reset();
	loadSMState();

}

void MenuHandler::file_open()
{    
	QString destination = 	QFileDialog::getOpenFileName(0," Open file", 
							QDir::currentPath(), 
							QString::fromUtf8("Blacksun files (*.bla)"));

	if(destination.length()==0)
		return;

	saveSMState();
	
	SceneManager *sm =SceneManager::getInstance();
    sm->loadScene(destination,true,false);
    
	loadSMState();

	m_file_savename = destination;
	sm->changeTitle(convertFilename(destination));
}

void MenuHandler::file_merge()
{    
	QString destination = 	QFileDialog::getOpenFileName(0," Open file", 
							QDir::currentPath(), 
							QString::fromUtf8("Blacksun files (*.bla)"));

	if(destination.length()==0)
		return;
		
	QMessageBox msgBox;
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setText("Should the current material settings kept?");
	
	bool keepMaterial;
	if(msgBox.exec() == QMessageBox::Yes)
		keepMaterial = true;
	else
		keepMaterial = false;
	
	SceneManager *sm =SceneManager::getInstance();
    sm->loadScene(destination,false,keepMaterial);
}

void MenuHandler::file_save()
{
	if(m_file_savename.length() == 0)
		this->file_save_as();
	else
		this->file_save_as(true);
			
}

void MenuHandler::file_save_as(bool resave )
{
	if(!resave)
	{
		m_file_savename = 	QFileDialog::getSaveFileName(0," Save file", 
							QDir::currentPath(), 	
							QString::fromUtf8("Blacksun files (*.bla)"));
												
		if(m_file_savename.length()==0)
			return;	
		
		if(!m_file_savename.endsWith(".bla"))
			m_file_savename.append(".bla");										
	}

	SceneManager *sm =SceneManager::getInstance();
    sm->saveScene(m_file_savename);
    
	sm->changeTitle(convertFilename(m_file_savename));
}

void MenuHandler::file_comment()
{
	SceneManager *sm =SceneManager::getInstance();
    QString text = sm->getDescription();
	
	bool ok;
	text = QInputDialog::getText(NULL, tr("Comment the scene"),
                                          tr("Please enter a scene description:"), QLineEdit::Normal,
                                          text, &ok);
	if (ok)
		 sm->setDescription(text);
}

void MenuHandler::edit_cut()
{
	edit_copy();
	edit_delete();
}

void MenuHandler::edit_copy()
{
	SceneManager *sm = SceneManager::getInstance();
 	SelectionBuffer &sb = sm->getSelBuffer();
	list<SceneObject*>& sObjects = sb.getSelectedObjects();
	
	m_savedMeshes.clear();
	m_savedPolys.clear();
	
	for(list<SceneObject*>::iterator it = sObjects.begin();it != sObjects.end(); ++it) 
   	{
		if( (*it)->getType() == SCENETYPE_Mesh)
		{
            Mesh *mesh = dynamic_cast<Mesh*>(*it);
            m_savedMeshes.push_back(*mesh);
		}
		
		if( (*it)->getType() == SCENETYPE_Polygon)
		{
            Polygon *poly = dynamic_cast<Polygon*>(*it);
            m_savedPolys.push_back(*poly);
		}
   	}
   	
   	sm->checkForRedraw(true);
}

void MenuHandler::edit_paste()
{
	SceneManager *sm =SceneManager::getInstance();
	
	for(vector<Mesh>::iterator it = m_savedMeshes.begin();it != m_savedMeshes.end(); ++it) 
   	{
   		sm->insertObject(*it);
   	}
   	
   	for(vector<Polygon>::iterator it = m_savedPolys.begin();it != m_savedPolys.end(); ++it) 
   	{
   		sm->insertObject(*it);
   	}
   	sm->checkForRedraw(true);
}

void MenuHandler::edit_delete()
{
	SceneManager *sm =SceneManager::getInstance();
	sm->getSelBuffer().deleteSelectedObjects();
    sm->checkForRedraw(true);
}


void MenuHandler::edit_clone()
{
	SceneManager *sm =SceneManager::getInstance();
	sm->getSelBuffer().copySelectedObjects();
    sm->checkForRedraw(true);
	
}

void MenuHandler::edit_mirror_x()
{
	SceneManager *sm =SceneManager::getInstance();
	sm->getSelBuffer().mirror(AXIS_x);
    sm->checkForRedraw(true);
}

void MenuHandler::edit_mirror_y()
{
	SceneManager *sm =SceneManager::getInstance();
	sm->getSelBuffer().mirror(AXIS_y);
    sm->checkForRedraw(true);
}

void MenuHandler::edit_mirror_z()
{
	SceneManager *sm =SceneManager::getInstance();
	sm->getSelBuffer().mirror(AXIS_z);
    sm->checkForRedraw(true);
}

void MenuHandler::edit_mirror_world_x()
{
	SceneManager *sm =SceneManager::getInstance();
	sm->getSelBuffer().mirrorWorld(AXIS_x);
    sm->checkForRedraw(true);
}

void MenuHandler::edit_mirror_world_y()
{
	SceneManager *sm =SceneManager::getInstance();
	sm->getSelBuffer().mirrorWorld(AXIS_y);
    sm->checkForRedraw(true);
}

void MenuHandler::edit_mirror_world_z()
{
	SceneManager *sm =SceneManager::getInstance();
	sm->getSelBuffer().mirrorWorld(AXIS_z);
    sm->checkForRedraw(true);
}

void MenuHandler::edit_meshFromPolygons()
{
	SceneManager *sm =SceneManager::getInstance();
	sm->getSelBuffer().mergePolysToMesh();
    sm->checkForRedraw(true);
}

void MenuHandler::edit_extractPolygonsFromMesh()
{
	SceneManager *sm =SceneManager::getInstance();
	sm->getSelBuffer().fragmentMeshs();
    sm->checkForRedraw(true);
}

void MenuHandler::edit_mergeMeshes()
{
	SceneManager *sm =SceneManager::getInstance();
	sm->getSelBuffer().mergeMeshes();
    sm->checkForRedraw(true);
}


void MenuHandler::selection_select_all()
{
	SceneManager *sm =SceneManager::getInstance();
    sm->getScene().selectAll(sm->getSelectionMode(),sm->getSelBuffer());
    sm->checkForRedraw(true);
}

void MenuHandler::selection_select_none()
{
	SceneManager *sm =SceneManager::getInstance();
	sm->getSelBuffer().reset();
    sm->checkForRedraw(true);
}

void MenuHandler::selection_select_invert()
{
	SceneManager *sm =SceneManager::getInstance();
    sm->getScene().selectInvert(sm->getSelectionMode(),sm->getSelBuffer());
    sm->checkForRedraw(true);
}

void MenuHandler::selection_select_intersecting()
{
	SceneManager *sm =SceneManager::getInstance();
	Aabb aabb = sm->getSelBuffer().getBoundary();
	
    sm->getScene().selectIntersecting(	sm->getSelectionMode(),sm->getSelBuffer(),
    									aabb,AXIS_x,SCENETYPE_Select);
    sm->checkForRedraw(true);
}

void MenuHandler::selection_hide_selection()
{
	SceneManager *sm =SceneManager::getInstance();
    sm->getScene().hideSelected(sm->getSelectionMode());
    sm->checkForRedraw(true);
}

void MenuHandler::selection_hide_all()
{
	SceneManager *sm =SceneManager::getInstance();
    sm->getScene().hideAll();
    sm->checkForRedraw(true);
}

void MenuHandler::selection_hide_none()
{
	SceneManager *sm =SceneManager::getInstance();
    sm->getScene().hideNone();
    sm->checkForRedraw(true);
}

void MenuHandler::selection_hide_invert()
{
	SceneManager *sm =SceneManager::getInstance();
    sm->getScene().hideInvert();
    sm->checkForRedraw(true);
}

void MenuHandler::selection_freeze_selection()
{
	SceneManager *sm =SceneManager::getInstance();
    sm->getScene().freezeSelected(sm->getSelectionMode());
    sm->checkForRedraw(true);
}

void MenuHandler::selection_freeze_all()
{
	SceneManager *sm =SceneManager::getInstance();
    sm->getScene().freezeAll();
    sm->checkForRedraw(true);
}

void MenuHandler::selection_freeze_none()
{
	SceneManager *sm =SceneManager::getInstance();
    sm->getScene().freezeNone();
    sm->checkForRedraw(true);
}

void MenuHandler::selection_freeze_invert()
{
	SceneManager *sm =SceneManager::getInstance();
    sm->getScene().freezeInvert();
    sm->checkForRedraw(true);
}

void MenuHandler::selection_ArrangeTogether()
{
	SceneManager *sm =SceneManager::getInstance();
	sm->getSelBuffer().arrangeTogether();
    sm->checkForRedraw(true);
}
	
void MenuHandler::selection_AlignXAxis()
{
	SceneManager *sm =SceneManager::getInstance();
	sm->getSelBuffer().alignAxis(AXIS_x);
    sm->checkForRedraw(true);
}	

void MenuHandler::selection_AlignYAxis()
{
	SceneManager *sm =SceneManager::getInstance();
	sm->getSelBuffer().alignAxis(AXIS_y);
    sm->checkForRedraw(true);
}	

void MenuHandler::selection_AlignZAxis()
{
	SceneManager *sm =SceneManager::getInstance();
	sm->getSelBuffer().alignAxis(AXIS_z);
    sm->checkForRedraw(true);
}	


void MenuHandler::selection_zoomTo()
{
	SceneManager *sm =SceneManager::getInstance();
	Aabb aabb = sm->getSelBuffer().getBoundary();
	
	if(isEmptyAabb(aabb))
		return;
		
	emit zoomToSelection(aabb);
}

void MenuHandler::selection_zoomAll()
{
	SceneManager *sm =SceneManager::getInstance();
	
	Aabb aabb;
	sm->getScene().getSceneAabb(aabb);
	
	if(isEmptyAabb(aabb))
		return;
		
	emit zoomToSelection(aabb);
}

bool MenuHandler::isEmptyAabb(const Aabb &aabb)
{
	const double eps = 0.0001;
	
	if((aabb.getMaximum().x - aabb.getMinimum().x < eps) &&
		(aabb.getMaximum().y - aabb.getMinimum().y < eps) &&
		(aabb.getMaximum().z - aabb.getMinimum().z < eps))
		return true;
	else
		return false;
}

void MenuHandler::modify_triangulate()
{
	SceneManager *sm =SceneManager::getInstance();
	
	QMessageBox msgBox;
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setText("Would you create single polygons?");
	
	bool single;
	if(msgBox.exec() == QMessageBox::Yes)
		single = true;
	else
		single = false;
	
	sm->getSelBuffer().triangulateVerts(single);
	
	sm->checkForRedraw(true);
}

void MenuHandler::modify_removeUnboundVerts()
{
	SceneManager *sm =SceneManager::getInstance();
    sm->getScene().removeAllVertices(sm->getSelBuffer());
    sm->checkForRedraw(true);
}

void MenuHandler::modify_turnVertexOrder()
{
	SceneManager *sm =SceneManager::getInstance();
	SelectionBuffer& sb = sm->getSelBuffer();
	sb.changeBias();
	
	sm->checkForRedraw(true);
}


void MenuHandler::material_coordinates()
{
	SceneManager *sm =SceneManager::getInstance();
	SelectionBuffer& sb = sm->getSelBuffer();
	double dtU=0.0f, dtV=0.0f, drU=0.0f, drV=0.0f;
	
	sb.getTextureTrans(&dtU,&dtV,&drU,&drV);
	
	qDebug() << "\n[Selected sets] Off: " << dtU << " , " << dtV << " , Rep: " << drU << " , " << drV;
	
	TextureOffRepDlg texCoordDlg;
	texCoordDlg.setCoordinates(dtU,dtV,drU,drV);
	     
    int result = texCoordDlg.exec();
    if (result == QDialog::Accepted)
    {
		texCoordDlg.getCoordinates(&dtU,&dtV,&drU,&drV);
		qDebug() << "[Dialog sets] Off: " << dtU << " , " << dtV << " , Rep: " << drU << " , " << drV;
		
		sb.transTextureCoords(dtU,dtV,drU,drV);
		qDebug() << "[New selected sets] Off: " << dtU << " , " << dtV << " , Rep: " << drU << " , " << drV;
    }
    
    sm->checkForRedraw(true);
}

void MenuHandler::material_remove()
{
	BSRenderer::MaterialManager* mm = BSRenderer::MaterialManager::getInstance();
	
	SceneManager *sm =SceneManager::getInstance();
	sm->getSelBuffer().setMaterial(mm->getStandardMaterialID());
	
    sm->checkForRedraw(true);
}
	
void MenuHandler::view_statistics()
{
	SceneManager *sm =SceneManager::getInstance();
    Scene &s = sm->getScene();
	
	QString stats;

    stats.append("<b>Details:</b>");
    stats.append("<table>");
    
    stats.append("<tr><td>Number of single polygons</td><td><strong>");
    stats.append( QString::number( s.countPolygons() ));
   	stats.append("</strong></td></tr>");
    
    stats.append("<tr><td>Number of meshes</td><td><strong>");
    stats.append( QString::number( s.countMeshes() ));
   	stats.append("</strong></td></tr>");
   	
   	stats.append("<tr><td>Number of single vertices</td><td><strong>");
    stats.append( QString::number( s.countSingleVertices() ));
   	stats.append("</strong></td></tr>");
   	
   	stats.append("<tr><td>Number of bound vertices</td><td><strong>");
    stats.append( QString::number( s.countBoundVertices(false) ));
   	stats.append("</strong></td></tr>");
    
    stats.append("<tr><td>Number of pure bound vertices  </td><td><strong>");
    stats.append( QString::number( s.countBoundVertices(true) ));
   	stats.append("</strong></td></tr>");
    
    stats.append("</table>");

    QMessageBox::about(NULL, "Scene statistics",  stats);
}

}
