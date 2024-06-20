#ifndef SFMLWIDGET_H
#define SFMLWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <SFML/Graphics.hpp>

class SFMLWidget : public QWidget {
    Q_OBJECT

public:
    explicit SFMLWidget(QWidget* parent = nullptr);
    ~SFMLWidget() override;

protected:
    void showEvent(QShowEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    QPaintEngine* paintEngine() const override;
    void resizeEvent(QResizeEvent* event) override;

private:
    sf::Texture mTextureMap;
    sf::Sprite mSpriteMap;
    sf::RenderWindow* mRenderWindow;
    //sf::View mView;
    sf::View getAdjustedView();

    bool SFMLloadTextureFromQtResource(sf::Texture& texture, const QString& resourcePath);
    void initializeSFML();
    void render();
};

#endif // SFMLWIDGET_H
