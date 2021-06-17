#pragma once

#include <algorithm>
#include <vector>
#include "global_defines.hpp"
#include <SFML/Graphics.hpp>

class GameManager;

/**
 * @brief      Base Class for All GameEntities
 */
class GameEntity {
private:
	// Less is higher priority

	/**
	 * @brief      Update Priority of this Entity
	 */
	int updatePriority;
	/**
	 * @brief      Stores whether the Update Priority was changed
	 */
	bool upTouched = true;
	/**
	 * @brief      Draw Priority of this Entity
	 */
	int drawPriority;
	
	/**
	 * @brief      Is this entity Alive?
	 */
	bool alive = true;
	
	/**
	 * @brief      Parent relative position of this Entity
	 */
	sf::Vector2f position;
	/**
	 * @brief      Size of this Entity
	 */
	sf::Vector2f size;
	/**
	 * @brief      Velocity of this Entity
	 */
	sf::Vector2f velocity;
	/**
	 * @brief      External/Control Acceleration of this Entity
	 */
	sf::Vector2f externalImpact;
	/**
	 * @brief      Parent Node of this Entity
	 */
	GameEntity* parentNode;
	/**
	 * @brief      Children Nodes of this Entity
	 */
	std::vector<GameEntity*> childNodes;

protected:
	/**
	 * @brief      GameManager pointer the active game that this Entity is part of
	 */
	GameManager *manager = nullptr;

	/**
	 * @brief      Default constructor, deleted
	 *
	 * @param      manager  The GameManager pointer
	 */
	GameEntity(GameManager* manager) = delete;

	/**
	 * @brief      Default Constructor for GameEntities
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 * @param[in]  u        Update Priority (default: 0)
	 * @param[in]  d        Draw Priority (default: 1)
	 */
	GameEntity(GameManager* manager, GameEntity* parent = nullptr, int u = 0, int d = 0) 
		: parentNode(parent), updatePriority(u), drawPriority(d), manager(manager) {
		if (parent) parent->addChild(this);
		setPosition(0, 0);
		setSize(1, 1);
		setVelocity(0, 0);
		resetExternalImpact();
	}
	
public:
	
	/**
	 * @brief      Destroys the object.
	 */
	virtual ~GameEntity() {
		for (GameEntity *entity : childNodes) delete entity;
	}
	
	/**
	 * @brief      Sets the update priority.
	 */
	void setUpdatePriority(int up) {updatePriority = up; upTouched = true;}
	
	/**
	 * @brief      Gets the update priority.
	 */
	int getUpdatePriority() const {return updatePriority;}
	
	/**
	 * @brief      Acknowledge Update Priority Changed
	 * 
	 * resets upTouched
	 */
	void acknowledgePriorityChange() {upTouched = false;}
	
	/**
	 * @brief      Has the Update Priority Changed?
	 */
	bool priorityChanged() const {return upTouched;}
	
	/**
	 * @brief      Sets the draw priority.
	 */
	void setDrawPriority(int dp) {drawPriority = dp;}
	
	/**
	 * @brief      Gets the draw priority.
	 */
	int getDrawPriority() const {return drawPriority;}

	/**
	 * @brief      Determines if alive.
	 *
	 * @return     True if alive, False otherwise.
	 */
	bool isAlive() const {return alive;}
	
	/**
	 * @brief      Sets this Entity Alive or Dead
	 *
	 * @param[in]  b     true for Alive, false for Dead
	 */
	void setAlive(bool b = true) {alive = b;}
	
	/**
	 * @brief      Gets the relative position of this Entity.
	 *
	 * @return     The position.
	 */
	sf::Vector2f getPosition() const {return position;}
	
	/**
	 * @brief      Sets the relative position of this Entity.
	 *
	 * @param[in]  pos   The new position
	 */
	virtual void setPosition(sf::Vector2f pos) {position = pos;}
	
	/**
	 * @brief      Sets the relative position of this Entity.
	 *
	 * @param[in]  x     X coordinate
	 * @param[in]  y     Y coordinate
	 */
	virtual void setPosition(float x, float y) {position = sf::Vector2f(x, y);}
	
	/**
	 * @brief      Gets the global position of this Entity
	 *
	 * @return     The global position (world coordinates)
	 */
	sf::Vector2f getTotalPosition() const {
		sf::Vector2f ret;
		for (const GameEntity *entity = this; entity != nullptr; entity = entity->getParentNode()) {
			ret += entity->getPosition();
		}
		return ret;
	}
	
	/**
	 * @brief      Gets the size of this Entity.
	 *
	 * @return     The size of this Entity.
	 */
	sf::Vector2f getSize() const {return size;}
	
	/**
	 * @brief      Sets the size of this Entity.
	 *
	 * @param[in]  sz    The size of this Entity
	 */
	virtual void setSize(sf::Vector2f sz) {size = sz;}
	
	/**
	 * @brief      Sets the size of this Entity.
	 *
	 * @param[in]  x     { parameter_description }
	 * @param[in]  y     { parameter_description }
	 */
	virtual void setSize(float x, float y) {size = sf::Vector2f(x, y);}
	
	/**
	 * @brief      Gets the velocity of this entity.
	 *
	 * @return     The velocity of this entity.
	 */
	sf::Vector2f getVelocity() const {return velocity;}
	
	/**
	 * @brief      Sets the velocity of this entity.
	 *
	 * @param[in]  vel   The velocity of this entity
	 */
	void setVelocity(sf::Vector2f vel) {velocity = vel;}
	
	/**
	 * @brief      Sets the velocity of this entity.
	 *
	 * @param[in]  x     { parameter_description }
	 * @param[in]  y     { parameter_description }
	 */
	void setVelocity(float x, float y) {velocity = sf::Vector2f(x, y);}
	
	/**
	 * @brief      Adds a certain amount of velocity instantaneously.
	 *
	 * @param[in]  acc   The acceleration (velocity to add)
	 */
	void addVelocity(sf::Vector2f acc) {velocity += acc;}
	
	/**
	 * @brief      Adds a certain amount of velocity instantaneously.
	 *
	 * @param[in]  x     x velocity
	 * @param[in]  y     y velocity
	 */
	void addVelocity(float x, float y) {velocity += sf::Vector2f(x, y);}
	
	/**
	 * @brief      Gets the external impact.
	 */
	sf::Vector2f getExternalImpact() const {return externalImpact;}
	
	/**
	 * @brief      Adds an external impact.
	 *
	 * @param[in]  acc   The acc
	 */
	void addExternalImpact(sf::Vector2f acc) {externalImpact += acc;}
	
	/**
	 * @brief      Adds an external impact.
	 *
	 * @param[in]  x     X acceleration
	 * @param[in]  y     Y acceleration
	 */
	void addExternalImpact(float x, float y) {externalImpact += sf::Vector2f(x, y);}
	
	/**
	 * @brief      Reset the external impact
	 */
	void resetExternalImpact() {externalImpact = sf::Vector2f(0, 0);}
	
	/**
	 * @brief      Gets the parent node.
	 */
	GameEntity* getParentNode() { return parentNode; }
	
	/**
	 * @brief      Gets the parent node.
	 */
	const GameEntity* getParentNode() const { return parentNode; }
	
	/**
	 * @brief      Gets the children nodes.
	 */
	std::vector<GameEntity*> & getChildNodes() { return childNodes; }
	
	/**
	 * @brief      Gets the children nodes.
	 */
	const std::vector<GameEntity*> & getChildNodes() const { return childNodes; }
	
	/**
	 * @brief      Attach this Entity to a Parent
	 *
	 * @param      p     new Parent Node
	 */
	void attachToParent(GameEntity *p) {
		if (parentNode != nullptr) {
			cerr << "Attaching entity to a parent while attacheR already attached" << endl;
			exit(-1);
		}
		parentNode = p;
		if (parentNode) {
			if (parentNode->hasChild(this)) {
				cerr << "Attaching entity to a parent while attacheE already attached" << endl;
				exit(-1);
			}
			parentNode->addChild(this);
		}
	}
	/**
	 * @brief      Detach this Entity from its Parent
	 */
	void detachFromParent() {
		if (parentNode) {
			if (!parentNode->hasChild(this)) {
				cerr << "Detaching entity from a parent but parent didnt have it as a child before" << endl;
				exit(-1);
			}
			parentNode->removeChild(this);
			parentNode = nullptr;
		}
	}
	
	/**
	 * @brief      Adds a child this Entity Node
	 *
	 * @param      child  The child
	 */
	void addChild(GameEntity *child) {
		child->acknowledgePriorityChange();
		childNodes.push_back(child);
		std::sort(childNodes.begin(), childNodes.end(), comparePtrToObj);
	}
	
	/**
	 * @brief      Removes a child from this Entity Node
	 *
	 * @param      child  The child
	 */
	void removeChild(GameEntity *child) {
		std::vector<GameEntity*>::iterator it = std::find(childNodes.begin(), childNodes.end(), child);
		if (it != childNodes.end()) childNodes.erase(it);
		// Order of elements still conserved.
	}
	
	/**
	 * @brief      Determines if the given Entity is a child of this Entity Node
	 *
	 * @param      other  The Entity Node
	 *
	 * @return     True if has child, False otherwise.
	 */
	bool hasChild(GameEntity *other) const {
		return std::find(childNodes.begin(), childNodes.end(), other) != childNodes.end();
	}
	
	/**
	 * @brief      Gets the Subtree of Children Nodes.
	 */
	std::vector<const GameEntity*> getSubtree() const {
		struct helper {
			void operator()(const GameEntity *x, std::vector<const GameEntity*> &vec) {
				vec.push_back(x);
				for (GameEntity *y : x->childNodes) (*this)(y, vec);
			}
		};
		std::vector<const GameEntity*> ret;
		helper()(this, ret);
		return ret;
	}
	

	/**
	 * @brief      Gets the Subtree of Children Nodes.
	 */
	std::vector<GameEntity*> getSubtree() {
		struct helper {
			void operator()(GameEntity *x, std::vector<GameEntity*> &vec) {
				vec.push_back(x);
				for (GameEntity *y : x->childNodes) (*this)(y, vec);
			}
		};
		std::vector<GameEntity*> ret;
		helper()(this, ret);
		return ret;
	}
	
	/**
	 * @brief      Update this node (if alive) and recursively update its children
	 */
	void updateNode() {	
		// End early if not alive
		if (!alive) return;
		
		// First, update itself
		update();
		
		// End early if not alive (again after update)
		if (!alive) return;
		
		// If there is any update priority change among childs
		if (std::any_of(childNodes.begin(), childNodes.end(), [](GameEntity *e){return e->priorityChanged();})) {
			// Re-sort vector
			std::sort(childNodes.begin(), childNodes.end(), comparePtrToObj);
			// Set all as clean
			std::for_each(childNodes.begin(), childNodes.end(), [](GameEntity *e){e->acknowledgePriorityChange();});
		}
		
		// Update childs (work on a copy or GG)
		std::vector<GameEntity*> childNodesCopy = childNodes;
		for (GameEntity *child : childNodesCopy) {
			// If still a child, update it
			if (hasChild(child)) child->updateNode();
		}
		
		// Remove dead
		for (int i = 0; i < childNodes.size(); i++) {
			if (!childNodes[i]->isAlive()) {
				delete childNodes[i];
				childNodes.erase(childNodes.begin()+i);
				i--;
			}
		}
	}
	
	/**
	 * @brief      Overridable update function to handle an Entities logic
	 * 
	 * Empty by default, no need to override for non active Entities
	 */
	virtual void update() {};
	
	/**
	 * @brief      Draws a node and all sub-nodes
	 * 
	 * not recursive
	 *
	 * @param      renderer  The RenderTarget to draw to
	 */
	void drawNode(sf::RenderTarget &renderer) const {
		std::vector<const GameEntity*> subtree = getSubtree();
		std::sort(subtree.begin(), subtree.end(), [](const GameEntity *a, const GameEntity *b){
			return (a->getDrawPriority() < b->getDrawPriority());
		});
		for (const GameEntity *x : subtree) {
			x->draw(renderer);
		}
	}
	
	/**
	 * @brief      Overridable draw function to handle an Entities graphics
	 *
	 *	Empty by default, no need to override for non-visible Entities
	 *
	 * @param      renderer  The RenderTarget to draw to
	 */
	virtual void draw(sf::RenderTarget &renderer) const { };
	
	
	/**
	 * @brief      Gets the gravity for this Entity
	 */
	virtual float getGravity() const {return 4.0f;}
	
	/**
	 * @brief      Compare Update Priorities
	 *
	 * @param[in]  other  The other GameEntity
	 *
	 * @return     true if this is lower update priority than the other
	 */
	bool operator<(const GameEntity &other) const {return updatePriority < other.updatePriority;}
	
	/**
	 * @brief      Compare Update Priorities
	 *
	 * @param[in]  a     GameEntity A
	 * @param[in]  b     GameEntity B
	 *
	 * @return     true if A is lower update priority than B
	 */
	static bool comparePtrToObj(const GameEntity *a, const GameEntity *b) { return *a < *b; }
};