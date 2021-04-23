

        
#include "./../include/TextureOffRepDlg.h"
#include "../include/SceneManager.h"
        
namespace BSScene
{
        
TextureOffRepDlg::TextureOffRepDlg()
    : QDialog()
{
	//Initialize the UI
	m_ui.setupUi(this);

	// Slider
	connect(m_ui.sliderOffsetU, SIGNAL(valueChanged (int)),
             this, SLOT(offsetUChanged(int)));  
	connect(m_ui.sliderOffsetV, SIGNAL(valueChanged (int)),
             this, SLOT(offsetVChanged(int)));
             
	// UpDown
	connect(m_ui.spinRepeatU, SIGNAL(valueChanged (int)),
             this, SLOT(repeatUChanged(int)));  
	connect(m_ui.spinRepeatV, SIGNAL(valueChanged (int)),
             this, SLOT(repeatVChanged(int)));                      
}

TextureOffRepDlg::~TextureOffRepDlg()
{
	
}

void TextureOffRepDlg::setCoordinates(const double offU, const double offV, const double repU,const  double repV)
{
	m_textureOffset[0]=offU;
	m_textureOffset[1]=offV;
	m_textureRepeat[0]=repU;
	m_textureRepeat[1]=repV;
	
	int dU = static_cast<int>(m_textureOffset[0] * 10);
	int dV = static_cast<int>(m_textureOffset[1] * 10);
	offsetUChanged(dU);
	offsetVChanged(dV);
	
	int drU = static_cast<int>(m_textureRepeat[0]);
	int drV = static_cast<int>(m_textureRepeat[1]);
	m_ui.spinRepeatU->setValue(drU);
	m_ui.spinRepeatV->setValue(drV);
}

void TextureOffRepDlg::getCoordinates(double *offU, double *offV, double *repU, double *repV) const
{
	if (offU) 
   		*offU = m_textureOffset[0];
   		
   	if (offV) 
   		*offV = m_textureOffset[1];
   		
   	if (repU) 
   		*repU = m_textureRepeat[0];
   		
   	if (repV) 
   		*repV = m_textureRepeat[1];
}

void TextureOffRepDlg::updateScene()  
{
	SceneManager *sm =SceneManager::getInstance();
    sm->checkForRedraw(true);
}

void TextureOffRepDlg::offsetUChanged(int val)
{
	double dVal = static_cast<double>(val) / 10.0;
	m_ui.spinOffsetU->setValue(dVal); 
	m_textureOffset[0] = dVal;
	
	updateScene() ;
}
    	
void TextureOffRepDlg::offsetVChanged(int val)
{
	double dVal = static_cast<double>(val) / 10.0;
	m_ui.spinOffsetV->setValue(dVal);
	m_textureOffset[1] = dVal;
	
	updateScene() ;
}

void TextureOffRepDlg::repeatUChanged(int val)
{
	m_textureRepeat[0] = val;
	
	updateScene() ;
}
    	
void TextureOffRepDlg::repeatVChanged(int val)
{
	m_textureRepeat[1] = val;
	
	updateScene() ;
}


}
