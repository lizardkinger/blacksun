#include "../include/Cmd_insertObject.h"
#include "../include/SceneManager.h"
#include <QMessageBox>

namespace BSScene
{
void Cmd_insertObject::redo()
{
	//QMessageBox::information(NULL, "REDO", "DER TEST");
	//SceneManager::getInstance()->insertObject(*m_newObject);
}
	
void Cmd_insertObject::undo()
{
	//QMessageBox::information(NULL, "UNDO", "DER TEST");
	SceneManager::getInstance()->removeObject(m_newObject);
}
}
