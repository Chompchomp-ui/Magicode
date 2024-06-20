#include "sfmlPreview.h"

#include <QFile>
#include <QByteArray>

SFMLPreview::SFMLPreview(QWidget* parent) :
    QWidget(parent),
    mRenderWindow(nullptr)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    setFocusPolicy(Qt::StrongFocus);
    //setFixedSize(300, 200);
}

SFMLPreview::~SFMLPreview() {
    if (mRenderWindow) {
        delete mRenderWindow;
    }
}

void SFMLPreview::showEvent(QShowEvent* event) {
    if (!mRenderWindow) {
        initializeSFML();
    }
    QWidget::showEvent(event);
}

void SFMLPreview::paintEvent(QPaintEvent* event) {
    (void)event;
    render();
}

QPaintEngine* SFMLPreview::paintEngine() const {
    return nullptr;
}

void SFMLPreview::resizeEvent(QResizeEvent* event) {
    if (mRenderWindow) {
        mRenderWindow->setSize(sf::Vector2u(width(), height()));
        mRenderWindow->setView(getAdjustedView());
    }
    QWidget::resizeEvent(event);
}

bool SFMLPreview::SFMLloadTextureFromQtResource(sf::Texture& texture, const QString& resourcePath)
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

void SFMLPreview::initializeSFML() {
    mRenderWindow = new sf::RenderWindow(reinterpret_cast<sf::WindowHandle>(winId()));
    mRenderWindow->setView(getAdjustedView());

    if (!SFMLloadTextureFromQtResource(mTexturePreview, ":/img/sol.bmp")) {
        QMessageBox::warning(this, "Cannot load texture", "Cannot load texture for the preview");
    }
}

sf::View SFMLPreview::getAdjustedView() {
    QWidget* myWidget = parentWidget()->findChild<QWidget*>("PreviewWidget");
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

void SFMLPreview::render() {
    mRenderWindow->clear(sf::Color::Black);

    mSpritePreview.setTexture(mTexturePreview);
    mRenderWindow->draw(mSpritePreview);

    mRenderWindow->display();
}
