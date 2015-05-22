#ifndef FRAME_H
#define FRAME_H

#include <memory>
#include <QImage>
#include "include/transformator.h"

struct Frame
{
    std::unique_ptr<TransformatorInterface> trator;
    std::unique_ptr<QImage> image;

    Frame(TransformatorInterface *trator, QImage *image):
        trator(trator), image(image) {}
};

#endif // FRAME_H

