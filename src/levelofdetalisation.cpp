#include "levelofdetalisation.h"

LevelOfDetalisation::LevelOfDetalisation(QString fileName, double upperLimit, double downLimit):
    rendererField(0),
    upperLimitField(upperLimit),
    downLimitField(downLimit)
{
    rendererField = new QSvgRenderer(fileName);
}

LevelOfDetalisation::~LevelOfDetalisation() throw()
{
    delete rendererField;
    rendererField = 0; //nullptr;
}
