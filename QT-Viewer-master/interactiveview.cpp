#include "interactiveview.h"
#include <QWheelEvent>
#include <QKeyEvent>
#include <QScrollBar>
#include <QDebug>

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH  viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

InteractiveView::InteractiveView(QWidget *parent)
    : QGraphicsView(parent),
      m_translateButton(Qt::LeftButton),
      m_scale(1.0),
      m_zoomDelta(0.1),
      m_translateSpeed(1.0),
      m_bMouseTranslate(false)
{

    // ?????????
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setCursor(Qt::PointingHandCursor);
    setRenderHint(QPainter::Antialiasing);

    //setSceneRect(parent->rect());
    centerOn(0, 0);

}

// ??????
void InteractiveView::setTranslateSpeed(qreal speed)
{
    // ????????¦¶
    Q_ASSERT_X(speed >= 0.0 && speed <= 2.0,
               "InteractiveView::setTranslateSpeed", "Speed should be in range [0.0, 2.0].");
    m_translateSpeed = speed;
}

qreal InteractiveView::translateSpeed() const
{
    return m_translateSpeed;
}

// ?????????
void InteractiveView::setZoomDelta(qreal delta)
{
    // ??????????¦¶
    Q_ASSERT_X(delta >= 0.0 && delta <= 1.0,
               "InteractiveView::setZoomDelta", "Delta should be in range [0.0, 1.0].");
    m_zoomDelta = delta;
}

qreal InteractiveView::zoomDelta() const
{
    return m_zoomDelta;
}

// ??/??/??/????????????????????/????????????????/????????
void InteractiveView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        translateF(QPointF(0, -2));  // ????
        break;
    case Qt::Key_Down:
        translateF(QPointF(0, 2));  // ????
        break;
    case Qt::Key_Left:
        translateF(QPointF(-2, 0));  // ????
        break;
    case Qt::Key_Right:
        translateF(QPointF(2, 0));  // ????
        break;
    case Qt::Key_Plus:  // ???
        zoomIn();
        break;
    case Qt::Key_Minus:  // ??§³
        zoomOut();
        break;
    case Qt::Key_Space:  // ????????
        rotate(-5);
        break;
    case Qt::Key_Enter:  // ???????
    case Qt::Key_Return:
        rotate(5);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

// ???
void InteractiveView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bMouseTranslate){
        setCursor (Qt::ClosedHandCursor);
        QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(m_lastMousePos);
        translateF(mouseDelta);
    }
    m_lastMousePos = event->pos();
    QGraphicsView::mouseMoveEvent(event);
}

void InteractiveView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == m_translateButton) {
        //  QPointF point = mapToScene(event->pos());
        //  if (scene()->itemAt(point, transform()) == NULL)  {  // ????????????? item ??????????
        m_bMouseTranslate = true;
        m_lastMousePos = event->pos();
        setCursor (Qt::ArrowCursor);
        //   }
    }
    QGraphicsView::mousePressEvent(event);
}

void InteractiveView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == m_translateButton){
        m_bMouseTranslate = false;
        setCursor (Qt::ArrowCursor);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

// ???/??§³
void InteractiveView::wheelEvent(QWheelEvent *event)
{
    setCursor(Qt::BusyCursor);
    QPoint scrollAmount = event->angleDelta();
    scrollAmount.y() > 0 ? zoomIn() : zoomOut();
    setCursor(Qt::ArrowCursor);
}

void InteractiveView::mouseDoubleClickEvent(QMouseEvent *event)
{
  //  centerOn (0,0);
}

// ???
void InteractiveView::zoomIn()
{
    zoom(1 + m_zoomDelta);
}

// ??§³
void InteractiveView::zoomOut()
{
    zoom(1 - m_zoomDelta);
}

// ???? - scaleFactor??????????????
void InteractiveView::zoom(float scaleFactor)
{
    m_scale *= scaleFactor;
    // ?????§³??????
//    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
//    if (factor < 0.07 || factor > 100)
//        return;
    QPointF oldPos = mapToScene (m_lastMousePos);
    scale(scaleFactor, scaleFactor);
    QPointF newPos = mapToScene (m_lastMousePos);
    translateF (newPos - oldPos);

}

// ???
void InteractiveView::translateF(QPointF delta)
{
    // ?????? zoom ?????????
    delta *= m_scale;
    delta *= m_translateSpeed;

    //  view ?????????¦Ì?????¨º??????¦Ë scene
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPoint newCenter(VIEW_WIDTH / 2 - delta.x(),  VIEW_HEIGHT / 2 - delta.y());
    centerOn(mapToScene(newCenter));

    //scene ?? view ??????????¨º??
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}


