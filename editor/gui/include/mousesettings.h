/***************************************************************************
*   Copyright (C) 2006 by Thomas Tischler
*   ttischl1@fh-landshut.de
*   
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*   
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*   
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the
*   Free Software Foundation, Inc.,
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*   (See COPYING for details.)
****************************************************************************
*
*  Module:     GUI (BlackSun)
*  File:       mousesettings.h
*  Created:    13.06.2007
*  Author:     Thomas Tischler (hunt0r)
*
**************************************************************************/


/** \file mousesettings.h
 * \brief Defines MouseSettings class.
 * 
 * \author Thomas Tischler.
 */

#ifndef MOUSESETTINGS_H
#define MOUSESETTINGS_H

#include <QtCore>
#include <QSettings>


namespace BSGui
{
        /*! \brief Stores mouse options.
         * This class acts like a backend for the mouse options. When this objet is created it reads the options from QSettings and when it is destroyed it saves them in QSettings
         */
class MouseSettings : public QObject
{
    
Q_OBJECT
Q_PROPERTY(MouseAction leftNoModifier3D READ getLeftNoModifier3D WRITE setLeftNoModifier3D)
Q_PROPERTY(MouseAction leftCtrlModifier3D READ getLeftCtrlModifier3D WRITE setLeftCtrlModifier3D)
Q_PROPERTY(MouseAction leftShiftModifier3D READ getLeftShiftModifier3D WRITE setLeftShiftModifier3D)
Q_PROPERTY(MouseAction leftCtrlShiftModifier3D READ getLeftCtrlShiftModifier3D WRITE setLeftCtrlShiftModifier3D)

Q_PROPERTY(MouseAction middleNoModifier3D READ getMiddleNoModifier3D WRITE setMiddleNoModifier3D)
Q_PROPERTY(MouseAction middleShiftModifier3D READ getMiddleShiftModifier3D WRITE setMiddleShiftModifier3D)
Q_PROPERTY(MouseAction middleCtrlModifier3D READ getMiddleCtrlModifier3D WRITE setMiddleCtrlModifier3D)
Q_PROPERTY(MouseAction middleCtrlShiftModifier3D READ getMiddleCtrlShiftModifier3D WRITE setMiddleCtrlShiftModifier3D)

/*Q_PROPERTY(MouseAction rightNoModifier3D READ getRightNoModifier3D WRITE setRightNoModifier3D)
Q_PROPERTY(MouseAction rightShiftModifier3D READ getRightShiftModifier3D WRITE setRightShiftModifier3D)
Q_PROPERTY(MouseAction rightCtrlModifier3D READ getRightCtrlModifier3D WRITE setRightCtrlModifier3D)
Q_PROPERTY(MouseAction rightCtrlShiftModifier3D READ getRightCtrlShiftModifier3D WRITE setRightCtrlShiftModifier3D)*/


Q_PROPERTY(MouseAction leftNoModifier2D READ getLeftNoModifier2D WRITE setLeftNoModifier2D)
Q_PROPERTY(MouseAction leftCtrlModifier2D READ getLeftCtrlModifier2D WRITE setLeftCtrlModifier2D)
Q_PROPERTY(MouseAction leftShiftModifier2D READ getLeftShiftModifier2D WRITE setLeftShiftModifier2D)
Q_PROPERTY(MouseAction leftCtrlShiftModifier2D READ getLeftCtrlShiftModifier2D WRITE setLeftCtrlShiftModifier2D)

Q_PROPERTY(MouseAction middleNoModifier2D READ getMiddleNoModifier2D WRITE setMiddleNoModifier2D)
Q_PROPERTY(MouseAction middleShiftModifier2D READ getMiddleShiftModifier2D WRITE setMiddleShiftModifier2D)
Q_PROPERTY(MouseAction middleCtrlModifier2D READ getMiddleCtrlModifier2D WRITE setMiddleCtrlModifier2D)
Q_PROPERTY(MouseAction middleCtrlShiftModifier2D READ getMiddleCtrlShiftModifier2D WRITE setMiddleCtrlShiftModifier2D)

/*Q_PROPERTY(MouseAction rightNoModifier2D READ getRightNoModifier2D WRITE setRightNoModifier2D)
Q_PROPERTY(MouseAction rightShiftModifier2D READ getRightShiftModifier2D WRITE setRightShiftModifier2D)
Q_PROPERTY(MouseAction rightCtrlModifier2D READ getRightCtrlModifier2D WRITE setRightCtrlModifier2D)
Q_PROPERTY(MouseAction rightCtrlShiftModifier2D READ getRightCtrlShiftModifier2D WRITE setRightCtrlShiftModifier2D)*/
Q_ENUMS(MouseAction)
    
    public:
        /*!
        * \brief The type specifies wich action to make on a mouse event
         */
        enum MouseAction
        {
            SelectedAction, /*!< The selected action should be executed*/
            Move, /*!< The camera should be moved*/
            Rotate, /*!< The camera should be rotated*/
            AddToSelection, /*!< Something should be added to the selection*/
            RemoveFromSelection, /*!< Something should be removed from the selection*/
            Nothing /*!< Nothing should be done*/
        };
        /*!
         * \brief Gets an instance of the mouse settings object
         * \return The MouseSettings object.
         */
        static MouseSettings* getInstance();
        /*!
         * \brief Sets the action for the specific modifier.
         * \param flag The action that should be done.
         */
        void setLeftNoModifier2D(MouseSettings::MouseAction flag){leftNoModifier2D = flag;}
        /*!
         * \brief Sets the action for the specific modifier.
         * \param flag The action that should be done.
         */
        void setLeftCtrlModifier2D(MouseSettings::MouseAction flag){leftCtrlModifier2D = flag;}
        /*!
         * \brief Sets the action for the specific modifier.
         * \param flag The action that should be done.
         */
        void setLeftShiftModifier2D(MouseSettings::MouseAction flag){leftShiftModifier2D = flag;}
        /*!
         * \brief Sets the action for the specific modifier.
         * \param flag The action that should be done.
         */
        void setLeftCtrlShiftModifier2D(MouseSettings::MouseAction flag){leftCtrlShiftModifier2D = flag;}
        
        void setMiddleNoModifier2D(MouseSettings::MouseAction flag){middleNoModifier2D = flag;}
        /*!
         * \brief Sets the action for the specific modifier.
         * \param flag The action that should be done.
         */
        void setMiddleShiftModifier2D(MouseSettings::MouseAction flag){middleShiftModifier2D = flag;}
        /*!
         * \brief Sets the action for the specific modifier.
         * \param flag The action that should be done.
         */
        void setMiddleCtrlModifier2D(MouseSettings::MouseAction flag){middleCtrlModifier2D = flag;}
        /*!
         * \brief Sets the action for the specific modifier.
         * \param flag The action that should be done.
         */
        void setMiddleCtrlShiftModifier2D(MouseSettings::MouseAction flag){middleCtrlShiftModifier2D = flag;}
        
        /*void setRightNoModifier2D(MouseSettings::MouseAction flag){rightNoModifier2D = flag;}
        void setRightShiftModifier2D(MouseSettings::MouseAction flag){rightShiftModifier2D = flag;}
        void setRightCtrlModifier2D(MouseSettings::MouseAction flag){rightCtrlModifier2D = flag;}
        void setRightCtrlShiftModifier2D(MouseSettings::MouseAction flag){rightCtrlShiftModifier2D = flag;}*/
        

        /*!
         * \brief Sets the action for the specific modifier.
         * \param flag The action that should be done.
         */
        void setLeftNoModifier3D(MouseSettings::MouseAction flag){leftNoModifier3D = flag;}
        /*!
         * \brief Sets the action for the specific modifier.
         * \param flag The action that should be done.
         */
        void setLeftCtrlModifier3D(MouseSettings::MouseAction flag){leftCtrlModifier3D = flag;}
        /*!
         * \brief Sets the action for the specific modifier.
         * \param flag The action that should be done.
         */
        void setLeftShiftModifier3D(MouseSettings::MouseAction flag){leftShiftModifier3D = flag;}
        /*!
         * \brief Sets the action for the specific modifier.
         * \param flag The action that should be done.
         */
        void setLeftCtrlShiftModifier3D(MouseSettings::MouseAction flag){leftCtrlShiftModifier3D = flag;}
        
        /*!
         * \brief Sets the action for the specific modifier.
         * \param flag The action that should be done.
         */
        void setMiddleNoModifier3D(MouseSettings::MouseAction flag){middleNoModifier3D = flag;}
        /*!
         * \brief Sets the action for the specific modifier.
         * \param flag The action that should be done.
         */
        void setMiddleShiftModifier3D(MouseSettings::MouseAction flag){middleShiftModifier3D = flag;}
        /*!
         * \brief Sets the action for the specific modifier.
         * \param flag The action that should be done.
         */
        void setMiddleCtrlModifier3D(MouseSettings::MouseAction flag){middleCtrlModifier3D = flag;}
        /*!
         * \brief Sets the action for the specific modifier.
         * \param flag The action that should be done.
         */
        void setMiddleCtrlShiftModifier3D(MouseSettings::MouseAction flag){middleCtrlShiftModifier3D = flag;}
        
        /*void setRightNoModifier3D(MouseSettings::MouseAction flag){rightNoModifier3D = flag;}
        void setRightShiftModifier3D(MouseSettings::MouseAction flag){rightShiftModifier3D = flag;}
        void setRightCtrlModifier3D(MouseSettings::MouseAction flag){rightCtrlModifier3D = flag;}
        void setRightCtrlShiftModifier3D(MouseSettings::MouseAction flag){rightCtrlShiftModifier3D = flag;}*/
        
        
        
        
        /*!
         * \brief Gets the action for the specific modifier.
         * \return The action of the modifier.
        */
        MouseSettings::MouseAction getLeftNoModifier2D() const {return leftNoModifier2D;} 
        /*!
         * \brief Gets the action for the specific modifier.
         * \return The action of the modifier.
         */
        MouseSettings::MouseAction getLeftCtrlModifier2D() const {return leftCtrlModifier2D;} 
        /*!
         * \brief Gets the action for the specific modifier.
         * \return The action of the modifier.
         */
        MouseSettings::MouseAction getLeftShiftModifier2D() const {return leftShiftModifier2D;} 
        /*!
         * \brief Gets the action for the specific modifier.
         * \return The action of the modifier.
         */
        MouseSettings::MouseAction getLeftCtrlShiftModifier2D() const {return leftCtrlShiftModifier2D;} 
        
        /*!
         * \brief Gets the action for the specific modifier.
         * \return The action of the modifier.
         */
        MouseSettings::MouseAction getMiddleNoModifier2D() const {return middleNoModifier2D;} 
        /*!
         * \brief Gets the action for the specific modifier.
         * \return The action of the modifier.
         */
        MouseSettings::MouseAction getMiddleShiftModifier2D() const {return middleShiftModifier2D;} 
        /*!
         * \brief Gets the action for the specific modifier.
         * \return The action of the modifier.
         */
        MouseSettings::MouseAction getMiddleCtrlModifier2D() const {return middleCtrlModifier2D;} 
        /*!
         * \brief Gets the action for the specific modifier.
         * \return The action of the modifier.
         */
        MouseSettings::MouseAction getMiddleCtrlShiftModifier2D() const {return middleCtrlShiftModifier2D;} 
        
        /*MouseSettings::MouseAction getRightNoModifier2D() const {return rightNoModifier2D;} 
        MouseSettings::MouseAction getRightShiftModifier2D() const {return rightShiftModifier2D;} 
        MouseSettings::MouseAction getRightCtrlModifier2D() const {return rightCtrlModifier2D;} 
        MouseSettings::MouseAction getRightCtrlShiftModifier2D() const {return rightCtrlShiftModifier2D;}*/
        

        
        MouseSettings::MouseAction getLeftNoModifier3D() const {return leftNoModifier3D;} 
        /*!
         * \brief Gets the action for the specific modifier.
         * \return The action of the modifier.
         */
        MouseSettings::MouseAction getLeftCtrlModifier3D() const {return leftCtrlModifier3D;} 
        /*!
         * \brief Gets the action for the specific modifier.
         * \return The action of the modifier.
         */
        MouseSettings::MouseAction getLeftShiftModifier3D() const {return leftShiftModifier3D;} 
        /*!
         * \brief Gets the action for the specific modifier.
         * \return The action of the modifier.
         */
        MouseSettings::MouseAction getLeftCtrlShiftModifier3D() const {return leftCtrlShiftModifier3D;} 
        
        /*!
         * \brief Gets the action for the specific modifier.
         * \return The action of the modifier.
         */
        MouseSettings::MouseAction getMiddleNoModifier3D() const {return middleNoModifier3D;} 
        /*!
         * \brief Gets the action for the specific modifier.
         * \return The action of the modifier.
         */
        MouseSettings::MouseAction getMiddleShiftModifier3D() const {return middleShiftModifier3D;} 
        /*!
         * \brief Gets the action for the specific modifier.
         * \return The action of the modifier.
         */
        MouseSettings::MouseAction getMiddleCtrlModifier3D() const {return middleCtrlModifier3D;} 
        /*!
         * \brief Gets the action for the specific modifier.
         * \return The action of the modifier.
         */
        MouseSettings::MouseAction getMiddleCtrlShiftModifier3D() const {return middleCtrlShiftModifier3D;} 
        
        /*MouseSettings::MouseAction getRightNoModifier3D() const {return rightNoModifier3D;} 
        MouseSettings::MouseAction getRightShiftModifier3D() const {return rightShiftModifier3D;} 
        MouseSettings::MouseAction getRightCtrlModifier3D() const {return rightCtrlModifier3D;} 
        MouseSettings::MouseAction getRightCtrlShiftModifier3D() const {return rightCtrlShiftModifier3D;}*/
        
        
    private:
        MouseSettings::MouseAction leftNoModifier3D; /*!< \brief Holds the modifier action */
        MouseSettings::MouseAction leftCtrlModifier3D; /*!< \brief Holds the modifier action */
        MouseSettings::MouseAction leftShiftModifier3D; /*!< \brief Holds the modifier action */
        MouseSettings::MouseAction leftCtrlShiftModifier3D; /*!< \brief Holds the modifier action */
        
        MouseSettings::MouseAction middleNoModifier3D; /*!< \brief Holds the modifier action */
        MouseSettings::MouseAction middleShiftModifier3D; /*!< \brief Holds the modifier action */
        MouseSettings::MouseAction middleCtrlModifier3D; /*!< \brief Holds the modifier action */
        MouseSettings::MouseAction middleCtrlShiftModifier3D; /*!< \brief Holds the modifier action */
        
        /*MouseSettings::MouseAction rightNoModifier3D;
        MouseSettings::MouseAction rightShiftModifier3D;
        MouseSettings::MouseAction rightCtrlModifier3D;
        MouseSettings::MouseAction rightCtrlShiftModifier3D;*/
        
        
        MouseSettings::MouseAction leftNoModifier2D; /*!< \brief Holds the modifier action */
        MouseSettings::MouseAction leftCtrlModifier2D; /*!< \brief Holds the modifier action */
        MouseSettings::MouseAction leftShiftModifier2D; /*!< \brief Holds the modifier action */
        MouseSettings::MouseAction leftCtrlShiftModifier2D; /*!< \brief Holds the modifier action */
        
        MouseSettings::MouseAction middleNoModifier2D; /*!< \brief Holds the modifier action */
        MouseSettings::MouseAction middleShiftModifier2D; /*!< \brief Holds the modifier action */
        MouseSettings::MouseAction middleCtrlModifier2D; /*!< \brief Holds the modifier action */
        MouseSettings::MouseAction middleCtrlShiftModifier2D; /*!< \brief Holds the modifier action */
        
        /*MouseSettings::MouseAction rightNoModifier2D;
        MouseSettings::MouseAction rightShiftModifier2D;
        MouseSettings::MouseAction rightCtrlModifier2D;
        MouseSettings::MouseAction rightCtrlShiftModifier2D;*/
        
        static MouseSettings* m_instance;  /*!< \brief Holds the instance of the class */
        class MouseSettingsGuardian 
        {
            public:
		/*!
             * \brief The destructor
        */
                virtual ~MouseSettingsGuardian() 
                {
                    if(MouseSettings::m_instance != NULL)
                        delete MouseSettings::m_instance;
                }
        };
        friend class MouseSettingsGuardian;
        
        QMap<QString, MouseAction> defaultActions;  /*!< \brief Maps the default actions to the modifiers */
        /*!
         * \brief The Constructor;
         */
        MouseSettings();
        /*!
         * \brief The Destructor;
         */
        virtual ~MouseSettings();
};

}

#endif
