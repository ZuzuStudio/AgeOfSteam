#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>

class GraphicalMap;
class FrameMemoisator;
class WorldView;

class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(GraphicalMap &model, QWidget *parent = nullptr);
    ~MapWidget();


protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    //bool event(QEvent *event);
private:
    FrameMemoisator *memoisator;
    WorldView *worldView;
};

#endif // MAPWIDGET_H
