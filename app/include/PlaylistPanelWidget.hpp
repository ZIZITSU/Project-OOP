#ifndef PLAYLISTPANELWIDGET_HPP
#define PLAYLISTPANELWIDGET_HPP

#include <QWidget>
#include <QString>

class PlaylistPanelWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit PlaylistPanelWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setStyleSheet("background-color: #252d3a;");
    }
    
signals:
    void playlistSelected(const QString& name);
};

#endif 