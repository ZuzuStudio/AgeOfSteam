#ifndef TRANSFORMATOR_H
#define TRANSFORMATOR_H

#include <QPointF>

class Area;

class TransformatorInterface
{
protected:
    TransformatorInterface() {}
public:
    TransformatorInterface(const TransformatorInterface &original) = delete;
    TransformatorInterface &operator=(const TransformatorInterface &rhs) = delete;
    TransformatorInterface(TransformatorInterface &&victim) = default;
    TransformatorInterface &operator=(TransformatorInterface &&rhs) = default;
    virtual ~TransformatorInterface() = 0;

    virtual qreal scale()const = 0;
    virtual QPointF transformToScreenCoordinates(const QPointF &point)const = 0;
    virtual QPointF transformToMapCoordinates(const QPointF &point)const = 0;
    virtual Area transformToScreenCoordinates(const Area &point)const = 0;
    virtual Area transformToMapCoordinates(const Area &point)const = 0;
};

class AbstractTransformator: public TransformatorInterface
{
protected:
    AbstractTransformator(const QPointF &mapViewCenter,
                          const QPointF &screenViewCenter,
                          const qreal &scale):
        mapViewCenter(mapViewCenter),
        screenViewCenter(screenViewCenter),
        scalePrivate(scale)
    {}

public:
    AbstractTransformator(const AbstractTransformator &original) = delete;
    AbstractTransformator &operator=(const AbstractTransformator &rhs) = delete;
    AbstractTransformator(AbstractTransformator &&victim) = default;
    AbstractTransformator &operator=(AbstractTransformator &&rhs) = default;
    ~AbstractTransformator() = 0;

    virtual qreal scale()const override final
    {
        return scalePrivate;
    }

    QPointF transformToScreenCoordinates(const QPointF &point)const override final;
    QPointF transformToMapCoordinates(const QPointF &point)const override final;
    Area transformToScreenCoordinates(const Area &area)const override final;
    Area transformToMapCoordinates(const Area &area)const override final;

private:
    const QPointF &mapViewCenter, &screenViewCenter;
    const qreal &scalePrivate;
};

class FollowingTransformator final: public AbstractTransformator
{
public:
    FollowingTransformator(const QPointF &mapViewCenter,
                           const QPointF &screenViewCenter,
                           const qreal &scale):
        AbstractTransformator(mapViewCenter, screenViewCenter, scale)
    {}

    FollowingTransformator(const FollowingTransformator &original) = delete;
    FollowingTransformator &operator=(const FollowingTransformator &rhs) = delete;
    FollowingTransformator(FollowingTransformator &&victim) = default;
    FollowingTransformator &operator=(FollowingTransformator &&rhs) = default;
    ~FollowingTransformator() = default;
};

class FixedScaleTransformator final: public AbstractTransformator
{
public:
    FixedScaleTransformator(const QPointF &mapViewCenter,
                            const QPointF &screenViewCenter,
                            qreal scale):
        AbstractTransformator(mapViewCenter, screenViewCenter, scalePrivate),
        scalePrivate(scale)
    {}

    FixedScaleTransformator(const FixedScaleTransformator &original) = delete;
    FixedScaleTransformator &operator=(const FixedScaleTransformator &rhs) = delete;
    FixedScaleTransformator(FixedScaleTransformator &&victim) = default;
    FixedScaleTransformator &operator=(FixedScaleTransformator &&rhs) = default;
    ~FixedScaleTransformator() = default;

private:
    qreal scalePrivate;
};

#endif // TRANSFORMATOR_H

