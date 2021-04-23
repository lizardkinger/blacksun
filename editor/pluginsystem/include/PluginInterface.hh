/*! \file PluginInterface.hh
 * \brief This is the interface for all plugins.
 * 
 * \author Stefan Zeltner.
 */

#ifndef PLUGININTERFACE_HH_
#define PLUGININTERFACE_HH_

#include <QString>
#include "BSBaseTypes.h"

using namespace BSPlgMgr;

/*! \brief The interface for every plugin.
 * 
 * In this interface are defiened the standart function which every plugin have.
 */
class PlgInt
{

public:

	/*! The standart constructor of PlgInt.
	 * 
	 * Note: This is an interface, you can not create ojects from this.
	 */
	PlgInt() {}
	
	/*! The destructor of PlgInt
	 */
	virtual ~PlgInt() {}

    // Functions, that every plugin must implement:
    
    /*! Returns the UID.
     * 
     * \return the UID of this plugin.
     */
    virtual UID getUID() = 0;
    
    /*! Returns the name.
     * 
     * \return the name of the plugin.
     */
    virtual QString getName() = 0;
    
    /*! Returns the autor's name.
     * 
     * \return the name of the autor.
     */
    virtual QString getAutor() = 0;
    
    /*! Returns the description.
     * 
     * Hyperlinks (http://... or mailto:...) will be translated into 'active'
     * hyperlinks, so the user can click it.
     * 
     * \return the description of the plugin.
     * */
    virtual QString getDescription() = 0;
    
    /*! Returns the version.
     * 
     * \return the version of the plugin.
     */
    virtual Version getVersion() = 0;
    
    /*! The install - function.
     * 
     * This is the first function, that the pluginsystem call.
     * 
     * \return say, if an error is occurred or not.
     */
    virtual bool installPlg() = 0;
    
    /*! The uninstall - function.
     * 
     * This function is called, if the Plugin is uninstalled by the user. It
     * can do here things (e.g. remove the plugin control in the gui), before
     * the Konstruktor is called.
     * 
     * \return say, if an error is occurred or not.
     */
    virtual bool uninstallPlg() = 0;
    
    /*! The load - function.
     * 
     * This function is called, before the Plugin is loaded. The Plugin should
     * take care which exist all required components, and this are ready to be
     * loaded. Here the Plugin should announce itself with the GUI.
     * 
     * \return say, if an error is occurred or not.
     */
    virtual bool loadPlg() = 0;
    
    /*! The unload - function.
     * 
     * This function is called, if the Plugin is unloaded by the user. Here
     * should the Plugin remove all it's control elements (e.g. from the GUI).
     * 
     * \return say, if an error is occurred or not.
     */
    virtual bool unloadPlg() = 0;
    
    /*! The close - function.
     * 
     * This function is called, if the application is finished from the user.
     * The Plugin should do their clear jobs (e.g. let of memory)
     * 
     */
    virtual void closePlg() = 0;
};

#endif /*PLUGININTERFACE_HH_*/
