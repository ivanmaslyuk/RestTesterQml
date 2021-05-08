#ifndef THEME_H
#define THEME_H

#include <QObject>

class Theme : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString primaryBackground READ primaryBackground WRITE setPrimaryBackground
               NOTIFY primaryBackgroundChanged)
    Q_PROPERTY(QString secondaryBackground READ secondaryBackground WRITE setSecondaryBackground
               NOTIFY secondaryBackgroundChanged)
    Q_PROPERTY(QString textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QString accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)
    Q_PROPERTY(QString lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)
    Q_PROPERTY(QString switchTabBarBg READ switchTabBarBg WRITE setSwitchTabBarBg NOTIFY switchTabBarBgChanged)
    Q_PROPERTY(QString treeViewHighlight READ treeViewHighlight WRITE setTreeViewHighlight
               NOTIFY treeViewHighlightChanged)
    Q_PROPERTY(QString secondaryTextColor READ secondaryTextColor WRITE setSecondaryTextColor
               NOTIFY secondaryTextColorChanged)
    Q_PROPERTY(QString toolButtonBg READ toolButtonBg WRITE setToolButtonBg NOTIFY toolButtonBgChanged)
    Q_PROPERTY(QString toolButtonFg READ toolButtonFg WRITE setToolButtonFg NOTIFY toolButtonFgChanged)

public:
    explicit Theme(QObject *parent = nullptr);

    QString primaryBackground() const;
    void setPrimaryBackground(QString value);

    QString secondaryBackground() const;
    void setSecondaryBackground(QString value);

    QString textColor() const;
    void setTextColor(QString value);

    QString accentColor() const;
    void setAccentColor(QString value);

    QString lineColor() const;
    void setLineColor(QString value);

    QString switchTabBarBg() const;
    void setSwitchTabBarBg(QString value);

    QString treeViewHighlight() const;
    void setTreeViewHighlight(QString value);

    QString secondaryTextColor() const;
    void setSecondaryTextColor(QString value);

    QString toolButtonBg() const;
    void setToolButtonBg(QString value);

    QString toolButtonFg() const;
    void setToolButtonFg(QString value);

private:
    QString m_primaryBackground;
    QString m_secondaryBackground;
    QString m_textColor;
    QString m_accentColor;
    QString m_lineColor;
    QString m_switchTabBarBg;
    QString m_treeViewHighlight;
    QString m_secondaryTextColor;
    QString m_toolButtonBg;
    QString m_toolButtonFg;

signals:
    void primaryBackgroundChanged(QString value);
    void secondaryBackgroundChanged(QString value);
    void textColorChanged(QString value);
    void accentColorChanged(QString value);
    void lineColorChanged(QString value);
    void switchTabBarBgChanged(QString value);
    void treeViewHighlightChanged(QString value);
    void secondaryTextColorChanged(QString value);
    void toolButtonBgChanged(QString value);
    void toolButtonFgChanged(QString value);
};

#endif // THEME_H
