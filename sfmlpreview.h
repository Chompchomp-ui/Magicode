#ifndef SFMLPREVIEW_H
#define SFMLPREVIEW_H

#include <QWidget>
#include <QMessageBox>
#include <SFML/Graphics.hpp>

class SFMLPreview : public QWidget {
    Q_OBJECT

public:
    explicit SFMLPreview(QWidget* parent = nullptr);
    ~SFMLPreview() override;

protected:
    void showEvent(QShowEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    QPaintEngine* paintEngine() const override;
    void resizeEvent(QResizeEvent* event) override;

private:
    sf::Texture mTexturePreview;
    sf::Sprite mSpritePreview;
    sf::RenderWindow* mRenderWindow;
    //sf::View mView;
    sf::View getAdjustedView();

    bool SFMLloadTextureFromQtResource(sf::Texture& texture, const QString& resourcePath);
    void initializeSFML();
    void render();
};

#endif // SFMLPREVIEW_H
