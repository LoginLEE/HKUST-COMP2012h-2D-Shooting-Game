#include "collision_box.hpp"

CollisionBox::CollisionBox() {}
CollisionBox::CollisionBox(sf::Vector2f center, sf::Vector2f size) {
	set(center, size);
}

void CollisionBox::set(sf::Vector2f center, sf::Vector2f size) {
	upLeft.x = center.x - size.x / 2.0f;
	upLeft.y = center.y - size.y / 2.0f;
	downRight.x = center.x + size.x / 2.0f;
	downRight.y = center.y + size.y / 2.0f;
	valid = (downRight.x > upLeft.x && downRight.y > upLeft.y);
}
void CollisionBox::setCenter(sf::Vector2f center) {
	set(center, downRight - upLeft);
}
void CollisionBox::setSize(sf::Vector2f size) {
	set((downRight + upLeft) / 2.0f, size);
}

sf::Vector2f CollisionBox::getCenter() const {
	return (downRight + upLeft) / 2.0f;
}
sf::Vector2f CollisionBox::getSize() const {
	return downRight - upLeft;
}
bool CollisionBox::isValid() const {
	return valid;
}

CollisionBox CollisionBox::intersect(const CollisionBox &other) const {
	return CollisionBox::intersect(*this, other);
}
bool CollisionBox::checkIntersection(const CollisionBox &other) const {
	return CollisionBox::checkIntersection(*this, other);
}

CollisionBox CollisionBox::intersect(const CollisionBox &a, const CollisionBox &b) {
	float left = std::max(a.upLeft.x, b.upLeft.x);
	float right = std::min(a.downRight.x, b.downRight.x);
	float up = std::max(a.upLeft.y, b.upLeft.y);
	float down = std::min(a.downRight.y, b.downRight.y);
	sf::Vector2f center((left+right)/2.0f, (up+down)/2.0f);
	sf::Vector2f size(right-left, down-up);
	return CollisionBox(center, size);
}
bool CollisionBox::checkIntersection(const CollisionBox &a, const CollisionBox &b) {
	return CollisionBox::intersect(a, b).valid;
}