#include "overhead_bar.hpp"

template <typename T>
void OverheadBar<T>::reposition() {
	if (getParentNode() == nullptr) {
		cerr << "Error: OverheadBar cannot be used without a parent" << endl;
		exit(-1);
	}
	GameEntity *parent = dynamic_cast<GameEntity*>(getParentNode());
	if (parent == nullptr) {
		setPosition( 0, -offset );
	} else {
		setPosition( 0, - parent->getSize().y / 2.0f - offset );
	}
}

template <typename T>
void OverheadBar<T>::draw(sf::RenderTarget &renderer) const {
	// Erroneous conditions
	if (current == nullptr || max == nullptr || *current > *max || *max == 0) return;

	float ratio = ((float)*current) / ((float)*max);
	
	// Don't display if full
	if ( ! DISPLAY_DEBUGGING_STUFF) {
		if (ratio == 1.0f) return;
		if (ratio < 0.0f) return; //or empty
	}
	sf::RectangleShape bar;	
	bar.setSize(sf::Vector2f(getSize().x * ratio, getSize().y));
	int redBegin = lowColor.r, redEnd = highColor.r;
	int greenBegin = lowColor.g, greenEnd = highColor.g;
	int blueBegin = lowColor.b, blueEnd = highColor.b;
	int redDelta = redEnd - redBegin;
	int greenDelta = greenEnd - greenBegin;
	int blueDelta = blueEnd - blueBegin;
	sf::Color interpolated = sf::Color( redBegin + ratio * redDelta, greenBegin + ratio * greenDelta, blueBegin + ratio * blueDelta );
	bar.setFillColor(interpolated);
	bar.setOrigin(getSize() / 2.0f);
	bar.setPosition(getTotalPosition());
	renderer.draw(bar);
}

template <typename T>
void OverheadBar<T>::update() {
	if (getParentNode() == nullptr) {
		cerr << "Error: OverheadBar cannot be used without a parent" << endl;
		exit(-1);
	}
}