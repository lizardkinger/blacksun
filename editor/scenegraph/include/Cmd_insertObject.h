#ifndef CMD_INSERTOBJECT_H_
#define CMD_INSERTOBJECT_H_

#include <QUndoCommand>

namespace BSScene
{
class SceneObject;

class Cmd_insertObject : public QUndoCommand
{
private:
	int m_cmdID;
	SceneObject* m_newObject;
	
public:
	Cmd_insertObject(SceneObject* newObject) : QUndoCommand("Insert Object"),
		m_cmdID(-1), m_newObject(newObject) {}
	virtual ~Cmd_insertObject() {}
	
	virtual int id () const {return m_cmdID;}
	virtual bool mergeWith ( const QUndoCommand * /*command*/ ) {return false;}
	virtual void redo ();
	virtual void undo ();
	
};
}

#endif /*CMD_INSERTOBJECT_H_*/
