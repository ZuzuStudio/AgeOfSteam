#ifndef LEVELOFDETALISATION_H
#define LEVELOFDETALISATION_H

#include <vector>
#include <QSvgRenderer>

class LevelOfDetalisation
{
public:
    LevelOfDetalisation(const qreal &referenceToCurrentScale);
    LevelOfDetalisation(const LevelOfDetalisation &)=delete;

    ~LevelOfDetalisation() noexcept;

    void addRenderer(const QString &fileName, qreal downLimit);
    QSvgRenderer *renderer() noexcept;
private:
    const qreal &currentScale;
    std::vector<QSvgRenderer *> renderers;
    std::vector<qreal> limits;
};

#endif // LEVELOFDETALISATION_H
