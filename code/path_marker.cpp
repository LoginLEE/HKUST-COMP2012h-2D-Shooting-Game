#include "path_marker.hpp"
#include "enemy.hpp"
#include "collision_box.hpp"

bool PathMarker::steppingIn(const Enemy* enemy) const {
	// return enemy->collidesWith(collisionBox);
	return collisionBox.containsPoint(enemy->getFeetPos());
}
