#ifndef LEVELOFDETALISATION_H
#define LEVELOFDETALISATION_H

#include <vector>
#include <QSvgRenderer>

class LevelOfDetalisation
{
public:
    LevelOfDetalisation();
    LevelOfDetalisation(const LevelOfDetalisation &) = delete;

    ~LevelOfDetalisation() noexcept;

    void addRenderer(const QString &fileName, qreal downLimit);
    QSvgRenderer *renderer(qreal currentScale) noexcept;
private:
    std::vector<QSvgRenderer *> renderers;
    std::vector<qreal> limits;
};

#endif // LEVELOFDETALISATION_H
