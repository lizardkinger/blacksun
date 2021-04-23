#include "../include/BSPlugin.h"

namespace BSPlgMgr
{

const char* BSPlugin::FUNC_NAME = "createInstance";

BSPlugin::BSPlugin(QByteArray fileName, bool state)
	: m_fileName(fileName), m_state(state),
	  m_handle(DynCompLoader::openLib((const char*)fileName)),
	  m_pPlugin(DynCompLoader::createInstance<PlgInt*>(m_handle, FUNC_NAME))
{
	if(m_pPlugin)
		m_UID = m_pPlugin->getUID();
}

BSPlugin::~BSPlugin()
{
	delete m_pPlugin;
	DynCompLoader::closeLib(m_handle);
}
}
