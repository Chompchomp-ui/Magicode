#include "sfmlwidget.h"

#include <QFile>
#include <QByteArray>

SFMLWidget::SFMLWidget(QWidget* parent) :
    QWidget(parent),
    mRenderWindow(nullptr)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    setFocusPolicy(Qt::StrongFocus);
    //setFixedSize(300, 200);
}

SFMLWidget::~SFMLWidget() {
    if (mRenderWindow) {
        delete mRenderWindow;
    }
}

void SFMLWidget::showEvent(QShowEvent* event) {
    if (!mRenderWindow) {
        initializeSFML();
    }
    QWidget::showEvent(event);
}

void SFMLWidget::paintEvent(QPaintEvent* event) {
    (void)event;
    render();
}

QPaintEngine* SFMLWidget::paintEngine() const {
    return nullptr;
}

void SFMLWidget::resizeEvent(QResizeEvent* event) {
    if (mRenderWindow) {
        mRenderWindow->setSize(sf::Vector2u(width(), height()));
        mRenderWindow->setView(getAdjustedView());
    }
    QWidget::resizeEvent(event);
}

bool SFMLWidget::SFMLloadTextureFromQtResource(sf::Texture& texture, const QString& resourcePath)
{
    QFile file(resourcePath);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(NULL, "Cannot load texture", "(!file.open(QIODevice::ReadOnly))");
        return false;
    }

    QByteArray data = file.readAll();
    if (data.isEmpty()) {
        QMessageBox::warning(NULL, "Cannot load texture", "(data.isEmpty())");
        return false;
    }

    return texture.loadFromMemory(data.constData(), data.size());
}

void SFMLWidget::initializeSFML() {
    mRenderWindow = new sf::RenderWindow(reinterpret_cast<sf::WindowHandle>(winId()));
    mRenderWindow->setView(getAdjustedView());

    /*if (!mTextureMap.loadFromFile("image.png")) {
        QMessageBox::warning(this, "Cannot load texture", "Cannot load texture for the map");
    }*/

    if (!SFMLloadTextureFromQtResource(mTextureMap, ":/img/image.png")) {
        QMessageBox::warning(this, "Cannot load texture", "Cannot load texture for the map");
    }
}

sf::View SFMLWidget::getAdjustedView() {
    QWidget* myWidget = parentWidget()->findChild<QWidget*>("MyWidget");
    if (!myWidget) {
        // Fallback if widget is not found
        return sf::View(sf::FloatRect(0, 0, 300, 200));
    }

    // Get the size of the widget
    float widgetWidth = myWidget->width();
    float widgetHeight = myWidget->height();

    sf::View view;
    float windowRatio = static_cast<float>(width()) / static_cast<float>(height());
    float viewRatio = widgetWidth / widgetHeight;

    if (windowRatio > viewRatio) {
        // Window is wider than the view
        float newWidth = widgetHeight * windowRatio;
        view.setSize(newWidth, widgetHeight);
    } else {
        // Window is taller than the view
        float newHeight = widgetWidth / windowRatio;
        view.setSize(widgetWidth, newHeight);
    }
    view.setCenter(widgetWidth / 2.f, widgetHeight / 2.f); // Center the view
    return view;
}

void SFMLWidget::render() {
    mRenderWindow->clear(sf::Color::Black);

    mSpriteMap.setTexture(mTextureMap);
    mRenderWindow->draw(mSpriteMap);

    mRenderWindow->display();
}
