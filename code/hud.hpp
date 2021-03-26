#pragma once

#include <SFML/Graphics.hpp>
#include "global_defines.hpp"
#include "game_entity.hpp"
#include "utils.hpp"

/**
 * @brief      Class for the Entity representing the HUD
 */
class HUD : public GameEntity {
private:

protected:

public:

	/**
	 * @brief      Create the HUD Entity
	 *
	 * @param      _manager  The global GameManager pointer
	 * @param      parent    The parent GameEntity node
	 */
	HUD(GameManager *_manager, GameEntity *parent);

	/**
	 * @brief      Update the HUD
	 */
	virtual void update() override;
	
	/**
	 * @brief      Draw the HUD
	 *
	 * @param      renderer  The RenderTarget to draw to
	 */
	virtual void draw(sf::RenderTarget &renderer) const override;
	
	/**
	 * @brief      font for the HUD
	 */
	static sf::Font entryFont;
	
	/**
	 * @brief      Constructs the Gun Info Pane as a Drawable Group
	 *
	 * @param      gun         The gun
	 * @param      compareGun  The gun to compare to
	 *
	 * @return     Drawable Group of Gun Info
	 */
	static utils::DrawableGroup craftGunInfo(Gun *gun, Gun *compareGun = nullptr);
	
	/**
	 * @brief      Construct the Mini Gun Info Pane as a Drawable Group
	 *
	 * @param      gun   The gun
	 *
	 * @return     Drawable Group of Gun Info
	 */
	static utils::DrawableGroup craftGunMiniInfo(Gun *gun);
};