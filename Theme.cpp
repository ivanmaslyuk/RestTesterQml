#include "Theme.h"

Theme::Theme(QObject *parent) : QObject(parent)
{

}

QString Theme::primaryBackground() const
{
    return m_primaryBackground;
}

void Theme::setPrimaryBackground(QString value)
{
    if (value == m_primaryBackground)
        return;

    m_primaryBackground = value;
    emit primaryBackgroundChanged(value);
}

QString Theme::secondaryBackground() const
{
    return m_secondaryBackground;
}

void Theme::setSecondaryBackground(QString value)
{
    if (value == m_secondaryBackground)
        return;

    m_secondaryBackground = value;
    emit secondaryBackgroundChanged(value);
}

QString Theme::textColor() const
{
    return m_textColor;
}

void Theme::setTextColor(QString value)
{
    if (value == m_textColor)
        return;

    m_textColor = value;
    emit textColorChanged(value);
}

QString Theme::accentColor() const
{
    return m_accentColor;
}

void Theme::setAccentColor(QString value)
{
    if (value == m_accentColor)
        return;

    m_accentColor = value;
    emit accentColorChanged(value);
}

QString Theme::lineColor() const
{
    return m_lineColor;
}

void Theme::setLineColor(QString value)
{
    if (value == m_lineColor)
        return;

    m_lineColor = value;
    emit lineColorChanged(value);
}

QString Theme::switchTabBarBg() const
{
    return m_switchTabBarBg;
}

void Theme::setSwitchTabBarBg(QString value)
{
    if (value == m_switchTabBarBg)
        return;

    m_switchTabBarBg = value;
    emit switchTabBarBgChanged(value);
}

QString Theme::treeViewHighlight() const
{
    return m_treeViewHighlight;
}

void Theme::setTreeViewHighlight(QString value)
{
    if (value == m_treeViewHighlight)
        return;

    m_treeViewHighlight = value;
    emit treeViewHighlightChanged(value);
}

QString Theme::secondaryTextColor() const
{
    return m_secondaryTextColor;
}

void Theme::setSecondaryTextColor(QString value)
{
    if (value == m_secondaryTextColor)
        return;

    m_secondaryTextColor = value;
    emit secondaryTextColorChanged(value);
}

QString Theme::toolButtonBg() const
{
    return m_toolButtonBg;
}

void Theme::setToolButtonBg(QString value)
{
    if (value == m_toolButtonBg)
        return;

    m_toolButtonBg = value;
    emit toolButtonBgChanged(value);
}

QString Theme::toolButtonFg() const
{
    return m_toolButtonFg;
}

void Theme::setToolButtonFg(QString value)
{
    if (value == m_toolButtonFg)
        return;

    m_toolButtonFg = value;
    emit toolButtonFgChanged(value);
}

