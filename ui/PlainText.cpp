#include "PlainText.h"

#include <QPainter>
#include <QTextDocument>
#include <QFontMetrics>

PlainText::PlainText(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_font("Consolas")
{
    setAcceptedMouseButtons(Qt::LeftButton);
    m_font.setPixelSize(12);
}

QString PlainText::text() const
{
    return m_text;
}

void PlainText::setText(QString text)
{
    m_text = text;

    QFontMetrics fontMetrics(m_font);
    setSize(fontMetrics.size(0, m_text));
    update();
}

void PlainText::paint(QPainter *painter)
{
    painter->setFont(m_font);
    painter->setPen(Qt::black);
    painter->drawText(boundingRect(), m_text);

}

void PlainText::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}
