#ifndef LEVELOFDETALISATION_H
#define LEVELOFDETALISATION_H

#include <QSvgRenderer>

class LevelOfDetalisation
{
    public:
        LevelOfDetalisation(QString fileName, double upperLimit, double downLimit);
        ~LevelOfDetalisation() throw();

        void setUpperLimit(double value)  throw() {upperLimitField = value;}
        double upperLimit() const throw() {return upperLimitField;}
        void setDownLimit(double value) throw() {downLimitField = value;}
        double downLimit() const throw() {return downLimitField;}
        const QSvgRenderer *renderer() const throw() {return rendererField;}
    private:
        QSvgRenderer *rendererField;
        double upperLimitField;
        double downLimitField;
};

#endif // LEVELOFDETALISATION_H
