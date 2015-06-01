#ifndef FRAME_H
#define FRAME_H

#include <memory>
#include <QImage>
#include "include/transformator.h"
#include "include/area.h"

class Frame final
{
public:
    Frame(TransformatorInterface *trator, QImage *image, const Area inScreenArea):
        tratorPrivate(trator), imagePrivate(image),
        previousInMapArea(tratorPrivate->transformToMapCoordinates(inScreenArea))
    {
    }

    const TransformatorInterface *trator()const
    {
        return tratorPrivate.get();
    }

    QImage *image()const
    {
        return imagePrivate.get();
    }

    void setImage(QImage *image)
    {
        imagePrivate.reset(image);
    }

    Area previousInScreenArea()const
    {
        return tratorPrivate->transformToScreenCoordinates(previousInMapArea);
    }

    void setPreviousArea(const Area &inScreenArea)
    {
        previousInMapArea = tratorPrivate->transformToMapCoordinates(inScreenArea);
    }

private:
    std::unique_ptr<TransformatorInterface> tratorPrivate;
    std::unique_ptr<QImage> imagePrivate;
    Area previousInMapArea;
};

#endif // FRAME_H

