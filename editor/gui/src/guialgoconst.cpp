#include "./../include/guialgoconst.h"

namespace BSGui
{
    
    QColor ColorToQColor(BSMath::Color theColor)
    {
        QColor result;
        result.setRedF(theColor.r);
        result.setGreenF(theColor.g);
        result.setBlueF(theColor.b);
        return result;
    }

    BSMath::Color QColorToColor(QColor theColor)
    {
        return BSMath::Color(theColor.redF(), theColor.greenF(), theColor.blueF());
    }
    
}
