#include "../include/levelofdetalisation.h"
#include <assert.h>

using namespace std;

LevelOfDetalisation::LevelOfDetalisation(const qreal &referenceToCurrentScale):
    currentScale(referenceToCurrentScale)
{
    // TODO Magic number move to config files
    renderers.reserve(4);
    limits.reserve(4);
    assert(renderers.size() == limits.size());
}

LevelOfDetalisation::~LevelOfDetalisation()noexcept
{
    assert(renderers.size() == limits.size());

    for(auto it : renderers)
        delete it;

}

void LevelOfDetalisation::addRenderer(const QString &fileName, qreal downLimit)
{
    assert((limits.size() == 0 && downLimit == 0.0)
           || (limits.size() > 0 && downLimit > limits.back()));
    renderers.push_back(new QSvgRenderer(fileName));
    limits.push_back(downLimit);
    assert(renderers.size() == limits.size());
}

QSvgRenderer *LevelOfDetalisation::renderer() noexcept
{
    assert(renderers.size() == limits.size());
    auto size = limits.size();

    if(size == 0)
        return nullptr;

    size_t i = 0;

    while(limits[i] < currentScale && i < size)
        ++i;

    return renderers[i - 1];
}
