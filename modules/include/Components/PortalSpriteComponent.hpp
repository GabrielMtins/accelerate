#ifndef PORTALSPRITECOMPONENT_HPP
#define PORTALSPRITECOMPONENT_HPP

struct PortalSpriteComponent {
	Vec3 position;
	Canvas *canvas;

	Vec3 internal_position;
	Vec3 internal_size;
};

#endif
