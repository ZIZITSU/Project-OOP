#ifndef RECOMMENDATIONPANELWIDGET_HPP
#define RECOMMENDATIONPANELWIDGET_HPP

#include <QWidget>
#include <QString>

class RecommendationPanelWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit RecommendationPanelWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setStyleSheet("background-color: #252d3a;");
    }
    
signals:
    void moodSelected(const QString& mood);
};

#endif