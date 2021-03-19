#ifndef PLAINTEXT_H
#define PLAINTEXT_H

#include <QQuickPaintedItem>
#include <QFont>

class PlainText : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
    QML_ELEMENT

public:
    PlainText(QQuickItem *parent = nullptr);

    QString text() const;
    void setText(QString text);

    void paint(QPainter *painter) override;

private:
    QString m_text;
    QFont m_font;

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // PLAINTEXT_H
