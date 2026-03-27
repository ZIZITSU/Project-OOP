
#include "MainWindow.hpp"
#include "AppController.hpp"
#include "PlayerBarWidget.hpp"
#include "PlaylistPanelWidget.hpp"
#include "RecommendationPanelWidget.hpp"
#include "SongRowWidget.hpp"
#include <QPainter>
#include <QPainterPath>
#include <QRadialGradient>
#include <QLinearGradient>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsDropShadowEffect>
#include <QRandomGenerator>
#include <cmath>

using namespace std;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), m_controller(nullptr), m_currentTab(0){
    setWindowTitle("PocketTrack - Your Music Companion");
    resize(1280, 800);
    setMinimumSize(1000, 600);
    
    // particle system:
    m_particleTimer = new QTimer(this);
    connect(m_particleTimer, &QTimer::timeout, this, &MainWindow::animateParticles);
    
    // Creating particles
    for (int i = 0; i < 20; ++i) {
        qreal x = QRandomGenerator::global()->bounded(width());
        qreal y = QRandomGenerator::global()->bounded(height());
        m_particlePositions.push_back(QPointF(x, y));
        m_particleOffsets.push_back(QRandomGenerator::global()->bounded(100) / 100.0);
    }
    
    setupUI();
    applyStyles();
    createAnimations();
    
    m_particleTimer->start(50); // Setting 20 FPS for particles
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    m_mainLayout = new QHBoxLayout(m_centralWidget);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
    
    setupSidebars();
    setupCentralWidget();
    setupPlayerBar();
}

void MainWindow::setupCentralWidget() {
    m_contentWidget = new QWidget();
    m_contentLayout = new QVBoxLayout(m_contentWidget);
    m_contentLayout->setContentsMargins(20, 20, 20, 20);
    m_contentLayout->setSpacing(20);
    
    setupTopBar();
    setupContentArea();
    
    m_mainLayout->addWidget(m_contentWidget, 1);
}

void MainWindow::setupTopBar() {
    m_topBar = new CardWidget();
    m_topBar->setFixedHeight(70);
    
    QHBoxLayout* topLayout = new QHBoxLayout(m_topBar);
    topLayout->setContentsMargins(15, 15, 15, 15);
    topLayout->setSpacing(15);
    
    // Online indicator
    m_onlineIndicator = new QLabel();
    m_onlineIndicator->setFixedSize(45, 45);
    m_onlineIndicator->setAlignment(Qt::AlignCenter);
    m_onlineIndicator->setText("🌐");
    m_onlineIndicator->setStyleSheet(
        "QLabel {"
        "   background-color: #00d9a5;"
        "   border-radius: 22px;"
        "   font-size: 24px;"
        "}"
    );
    
    // Pulse animation for online indicator
    QGraphicsOpacityEffect* pulseEffect = new QGraphicsOpacityEffect(m_onlineIndicator);
    m_onlineIndicator->setGraphicsEffect(pulseEffect);
    QPropertyAnimation* pulseAnim = new QPropertyAnimation(pulseEffect, "opacity");
    pulseAnim->setDuration(2000);
    pulseAnim->setStartValue(0.5);
    pulseAnim->setEndValue(1.0);
    pulseAnim->setEasingCurve(QEasingCurve::InOutSine);
    pulseAnim->setLoopCount(-1);
    pulseAnim->start(QAbstractAnimation::DeleteWhenStopped);
    
    topLayout->addWidget(m_onlineIndicator);
    
    // Search bar
    m_searchBar = new QLineEdit();
    m_searchBar->setPlaceholderText("Search songs, artists, or albums...");
    m_searchBar->setStyleSheet(
        "QLineEdit {"
        "   background-color: rgba(45, 55, 72, 0.5);"
        "   border: none;"
        "   border-radius: 22px;"
        "   padding: 12px 20px;"
        "   color: #ffffff;"
        "   font-size: 14px;"
        "}"
        "QLineEdit::placeholder {"
        "   color: #a0a0a0;"
        "}"
    );
    connect(m_searchBar, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    
    topLayout->addWidget(m_searchBar, 1);
    
    // View mode buttons
    QPushButton* gridViewBtn = new QPushButton("📊");
    gridViewBtn->setFixedSize(45, 45);
    gridViewBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #2d3748;"
        "   border: none;"
        "   border-radius: 22px;"
        "   color: #ffffff;"
        "   font-size: 20px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #3d4758;"
        "}"
    );
    topLayout->addWidget(gridViewBtn);
    
    // Settings button:
    m_settingsBtn = new QPushButton("⚙️");
    m_settingsBtn->setFixedSize(45, 45);
    m_settingsBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #2d3748;"
        "   border: none;"
        "   border-radius: 22px;"
        "   color: #ffffff;"
        "   font-size: 20px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #3d4758;"
        "}"
    );
    topLayout->addWidget(m_settingsBtn);
    
    m_contentLayout->addWidget(m_topBar);
}


void MainWindow::onSongPlayRequested(shared_ptr<Song> song) {
    // Explicit Guard Clauses
    if (song == nullptr) {
        return;
    }
    
    if (m_playerBar == nullptr) {
        return;
    }
    
    // Explicit Artist extraction
    QString artistName;
    if (song->getArtist() != nullptr) {
        artistName = QString::fromStdString(song->getArtist()->getName());
    } 
    else {
        artistName = "Unknown";
    }
    
    // Explicit Album extraction
    QString albumTitle;
    if (song->getAlbum() != nullptr) {
        albumTitle = QString::fromStdString(song->getAlbum()->getTitle());
    } 
    else {
        albumTitle = "";
    }
    
    // Updating player bar with the variables we explicitly set
    m_playerBar->setCurrentSong(
        QString::fromStdString(song->getTitle()),artistName,albumTitle
    );
    
    
    // Set duration
    m_playerBar->setDuration(song->getDuration());
    
    
    // Start playing
    m_playerBar->setPlaying(true);
    

    // Updating all song rows to show which is playing
    for (int i = 0; i < m_songListLayout->count(); ++i) {
        SongRowWidget* row = qobject_cast<SongRowWidget*>(m_songListLayout->itemAt(i)->widget());
        
        if (row != nullptr) {
            // Explicitly setting the boolean rather than relying on an inline equation
            if (row->getSongId() == song->getId()) {
                row->setPlaying(true);
            } 
            else {
                row->setPlaying(false);
            }
        }
    }
}


void MainWindow::setupContentArea() {
    m_contentArea = new CardWidget();
    
    QVBoxLayout* contentAreaLayout = new QVBoxLayout(m_contentArea);
    contentAreaLayout->setContentsMargins(20, 20, 20, 20);
    contentAreaLayout->setSpacing(15);
    
    // Header with tabs
    QHBoxLayout* headerLayout = new QHBoxLayout();
    
    QLabel* libraryLabel = new QLabel("Your Library");
    libraryLabel->setStyleSheet(
        "QLabel {"
        "   color: #ffffff;"
        "   font-size: 28px;"
        "   font-weight: bold;"
        "}"
    );
    headerLayout->addWidget(libraryLabel);
    headerLayout->addStretch();
    
    // Tabs
    m_tabWidget = new QWidget();
    m_tabLayout = new QHBoxLayout(m_tabWidget);
    m_tabLayout->setSpacing(10);
    m_tabLayout->setContentsMargins(0, 0, 0, 0);
    
    QStringList tabNames = {"All Songs", "Artists", "Albums", "Genres"};
    for (int i = 0; i < tabNames.size(); ++i) {
        TabButton* tab = new TabButton(tabNames[i]);
        //if (i == 0) tab->setActive(true);
        
        connect(tab, &QPushButton::clicked, this, [this, i]() {
            onTabChanged(i);
        });
        
        m_tabs.push_back(tab);
        m_tabLayout->addWidget(tab);
    }
    
    headerLayout->addWidget(m_tabWidget);
    contentAreaLayout->addLayout(headerLayout);
    
    // Song list scroll area
    m_songScrollArea = new QScrollArea();
    m_songScrollArea->setWidgetResizable(true);
    m_songScrollArea->setFrameShape(QFrame::NoFrame);
    m_songScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_songScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    m_songListContainer = new QWidget();
    m_songListLayout = new QVBoxLayout(m_songListContainer);
    m_songListLayout->setSpacing(8);
    m_songListLayout->setContentsMargins(0, 0, 0, 0);
    m_songListLayout->addStretch();
    
    m_songScrollArea->setWidget(m_songListContainer);
    m_songScrollArea->setStyleSheet(
        "QScrollArea {"
        "   background: transparent;"
        "   border: none;"
        "}"
        "QScrollBar:vertical {"
        "   background: rgba(45, 55, 72, 0.3);"
        "   width: 8px;"
        "   border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: #e94560;"
        "   border-radius: 4px;"
        "   min-height: 20px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "   background: #d32f4f;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "   height: 0px;"
        "}"
    );
    
    contentAreaLayout->addWidget(m_songScrollArea);
    
    m_contentLayout->addWidget(m_contentArea, 1);
}

void MainWindow::setupSidebars() {
    m_playlistPanel = new PlaylistPanelWidget();
    m_playlistPanel->setFixedWidth(250);
    connect(m_playlistPanel, &PlaylistPanelWidget::playlistSelected,
            this, &MainWindow::onPlaylistSelected);
    
    m_mainLayout->addWidget(m_playlistPanel);
    
    m_recommendationPanel = new RecommendationPanelWidget();
    m_recommendationPanel->setFixedWidth(320);
    connect(m_recommendationPanel, &RecommendationPanelWidget::moodSelected,
            this, &MainWindow::onMoodFilterSelected);
}

void MainWindow::setupPlayerBar() {
    m_playerBar = new PlayerBarWidget();
    m_playerBar->setFixedHeight(100);
    
    // Adding player bar as a separate widget at the bottom
    // used a QVBoxLayout in central widget
    QVBoxLayout* mainVerticalLayout = new QVBoxLayout();
    mainVerticalLayout->setContentsMargins(0, 0, 0, 0);
    mainVerticalLayout->setSpacing(0);
    
    // Creating a container for the main layout
    QWidget* contentContainer = new QWidget();
    contentContainer->setLayout(m_mainLayout);
    
    mainVerticalLayout->addWidget(contentContainer, 1);
    mainVerticalLayout->addWidget(m_playerBar);
    
    m_centralWidget->setLayout(mainVerticalLayout);
}

void MainWindow::applyStyles() {
    setStyleSheet(
        "QMainWindow {"
        "   background: qlineargradient("
        "       x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #1a1a2e,"
        "       stop:0.5 #16213e,"
        "       stop:1 #0f3460"
        "   );"
        "}"
    );
}

void MainWindow::createAnimations() {
    // Slide-in animation for sidebars
    QPropertyAnimation* leftSlide = new QPropertyAnimation(m_playlistPanel, "pos");
    leftSlide->setDuration(600);
    leftSlide->setEasingCurve(QEasingCurve::OutCubic);
    
    QPropertyAnimation* rightSlide = new QPropertyAnimation(m_recommendationPanel, "pos");
    rightSlide->setDuration(600);
    rightSlide->setEasingCurve(QEasingCurve::OutCubic);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QMainWindow::paintEvent(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Drawing gradient background
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0, COLOR_BG_DARK);
    gradient.setColorAt(0.5, COLOR_BG_MEDIUM);
    gradient.setColorAt(1.0, COLOR_BG_LIGHT);
    painter.fillRect(rect(), gradient);
    
    // Drawing animated particles
    painter.setPen(Qt::NoPen);
    for (size_t i = 0; i < m_particlePositions.size(); ++i) {
        QColor particleColor = COLOR_PRIMARY;
        particleColor.setAlphaF(0.3 + 0.3 * sin(m_particleOffsets[i]));
        painter.setBrush(particleColor);
        
        QPointF pos = m_particlePositions[i];
        painter.drawEllipse(pos, 2, 2);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    // Reinitializing particles on resize
}

void MainWindow::animateParticles() {
    for (size_t i = 0; i < m_particlePositions.size(); ++i) {
        m_particleOffsets[i] += 0.05;
        m_particlePositions[i].ry() -= 1.0; // Moving up
        
        // Resetting if off screen
        if (m_particlePositions[i].y() < -20) {
            m_particlePositions[i].setY(height() + 20);
            m_particlePositions[i].setX(QRandomGenerator::global()->bounded(width()));
        }
    }
    update();
}

void MainWindow::setController(AppController* controller) {
    m_controller = controller;
    if (m_playerBar) {
        m_playerBar->setController(controller);
    }
}

void MainWindow::updateSongList() {
    if (!m_controller) return;
    
    // Clear existing songs
    QLayoutItem* child;
    while ((child = m_songListLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    
    // Getting songs from controller
    vector<shared_ptr<Song>> songs;
    
    if (!m_currentFilter.isEmpty()) {
        songs = m_controller->searchSongs(m_currentFilter);
    } 
    else if (!m_currentMood.isEmpty()) {
        songs = m_controller->filterByMood(m_currentMood);
    } 
    else {
        songs = m_controller->getAllSongs();
    }
    
    int displayCount = min(static_cast<int>(songs.size()), 50);
    
    for (int i = 0; i < displayCount; ++i) {
        const auto& song = songs[i];
        
        SongRowWidget* songRow = new SongRowWidget();
        songRow->setSongInfo(
            QString::fromStdString(song->getTitle()),
            QString::fromStdString(song->getArtist() ? song->getArtist()->getName() : "Unknown"),
            QString::fromStdString(song->getAlbum() ? song->getAlbum()->getTitle() : "Unknown"),
            song->getDuration(),
            song->getRating()
        );
        songRow->setSongId(song->getId());
        songRow->setFavorite(song->isFavorite());
        
        // Connecting song click to play
        connect(songRow, &SongRowWidget::playRequested, this, [this, song]() {
            onSongPlayRequested(song);
        });
        
        // Connecting double-click to play
        connect(songRow, &SongRowWidget::doubleClicked, this, [this, song]() {
            onSongPlayRequested(song);
        });
        
        m_songListLayout->insertWidget(m_songListLayout->count() - 1, songRow);
        
        // Staggered animation
        songRow->setOpacity(0);
        QPropertyAnimation* fadeIn = new QPropertyAnimation(songRow, "opacity");
        fadeIn->setDuration(400);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);
        fadeIn->setEasingCurve(QEasingCurve::OutCubic);
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
    }
    
    m_songListLayout->addStretch();
}


void MainWindow::onSearchTextChanged(const QString& text) {
    m_currentFilter = text;
    updateSongList();
}

void MainWindow::onTabChanged(int index) {
    for (size_t i = 0; i < m_tabs.size(); ++i) {
        //m_tabs[i]->setActive(i == index);
    }
    m_currentTab = index;
    updateSongList();
}

void MainWindow::onSongsLoaded() {
    updateSongList();
}

void MainWindow::onPlaylistSelected(const QString& name) {
    // Filtering songs by playlist
    updateSongList();
}

void MainWindow::onMoodFilterSelected(const QString& mood) {
    m_currentMood = mood;
    updateSongList();
}

void MainWindow::showWelcomeScreen() {
    // Implementing welcome screen
}

void MainWindow::showLibrary() {
    updateSongList();
}


// CardWidget Implementation

CardWidget::CardWidget(QWidget *parent) : QWidget(parent), m_cornerRadius(15) , m_cardColor(QColor("#252d3a")), m_hoverEnabled(false)
    , m_isHovered(false) , m_opacity(1.0){

    setAttribute(Qt::WA_TranslucentBackground);
    
    m_hoverAnimation = new QPropertyAnimation(this, "opacity");
    m_hoverAnimation->setDuration(200);
    m_hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);
}



void CardWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setOpacity(m_opacity);
    
    // Drawing card background with rounded corners
    QColor bgColor = m_cardColor;
    if (m_isHovered && m_hoverEnabled) {
        bgColor = bgColor.lighter(110);
    }
    
    painter.setBrush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), m_cornerRadius, m_cornerRadius);
}

void CardWidget::enterEvent(QEnterEvent *event) {
    if (m_hoverEnabled) {
        m_isHovered = true;
        update();
    }
    QWidget::enterEvent(event);
}

void CardWidget::leaveEvent(QEvent *event) {
    if (m_hoverEnabled) {
        m_isHovered = false;
        update();
    }
    QWidget::leaveEvent(event);
}


// TabButton Implementation
TabButton::TabButton(const QString& text, QWidget *parent): QPushButton(text, parent), m_isActive(false), m_isHovered(false), m_scale(1.0)
{
    setFixedSize(100, 40);
    setCursor(Qt::PointingHandCursor);
    
    m_animation = new QPropertyAnimation(this, "scale");
    m_animation->setDuration(150);
    m_animation->setEasingCurve(QEasingCurve::OutCubic);
}


void TabButton::setActive(bool active) {
    m_isActive = active;
    update();
}


void TabButton::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Background
    QColor bgColor = m_isActive ? QColor("#e94560") : QColor("#0f3460");
    if (m_isHovered && !m_isActive) {
        bgColor = bgColor.lighter(120);
    }
    
    painter.setBrush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 20, 20);
    
    // Text
    painter.setPen(QColor("#ffffff"));
    QFont font = painter.font();
    font.setPixelSize(13);
    font.setBold(m_isActive);
    painter.setFont(font);
    painter.drawText(rect(), Qt::AlignCenter, text());
}

void TabButton::enterEvent(QEnterEvent *event) {
    m_isHovered = true;
    update();
    QPushButton::enterEvent(event);
}

void TabButton::leaveEvent(QEvent *event) {
    m_isHovered = false;
    update();
    QPushButton::leaveEvent(event);
}
