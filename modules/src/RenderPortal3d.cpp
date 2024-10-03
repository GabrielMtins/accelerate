#include "Systems/RenderPortal3d.hpp"
#include "Game.hpp"

#define CLIP_DISTANCE 0.25f

namespace acc {

RenderPortal3d::RenderPortal3d(ScenePortal3d *scene){
	this->scene = scene;

	framebuffer_canvas = scene->getFramebufferCanvas();
	framebuffer_texture = scene->getFramebufferTexture();
	tilemap_canvas = (Canvas *) scene->getGame()->getResource("tile_canvas.png");

	tile_width = tile_height = 64;

	camera_position = Vec3(1.0f, 1.0f, 0.0f);
	camera_angle = -0.8f;
	camera_sector = 0;

	framebuffer_width = framebuffer_canvas->getWidth();
	framebuffer_height = framebuffer_canvas->getHeight();

	div_table_x = new float[framebuffer_width];
	div_table_y = new float[framebuffer_height];
	
	top_value_y = new float[framebuffer_width];
	bottom_value_y = new float[framebuffer_width];

	depth_buffer = new float[framebuffer_width * framebuffer_height];

	precalc();
}

void RenderPortal3d::update(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<PortalWorldComponent>())
		return;

	auto arr = component_manager->getComponentArray<PortalWorldComponent>();

	for(size_t i = 0; i < arr->getSize(); i++)
		render(arr->atIndex(i));
}

RenderPortal3d::~RenderPortal3d(void){
	delete div_table_x;
	delete div_table_y;
	delete top_value_y;
	delete bottom_value_y;
	delete depth_buffer;
}

void RenderPortal3d::precalc(void){
	for(int i = 0; i < framebuffer_height; i++){
		if(i == framebuffer_height / 2){
			div_table_y[i] = 1000000;
			continue;
		}

		div_table_y[i] = 
			(float) framebuffer_height / (i - framebuffer_height / 2) / 2;
	}

	for(int i = 0; i < framebuffer_width; i++){
		div_table_x[i] = 
			(float) (i - framebuffer_width / 2) / (framebuffer_height / 2);
	}
}

void RenderPortal3d::clearValuesY(void){
	for(int i = 0; i < framebuffer_width; i++)
		top_value_y[i] = framebuffer_height;

	for(int i = 0; i < framebuffer_width; i++)
		bottom_value_y[i] = 0;
}

void RenderPortal3d::clearDepthBuffer(void){
	memset(depth_buffer, 0, sizeof(float) * framebuffer_width * framebuffer_height);
}

void RenderPortal3d::updateCameraPosition(PortalWorldComponent& world, Vec3 new_position){
	for(Wall& current_wall : world.sectors[camera_sector].walls){
		if(current_wall.portal == -1) continue;

		Vec3 direction_wall = current_wall.b - current_wall.a;
		Vec3 ortho_direction_wall = Vec3(direction_wall.y, -direction_wall.x, 0.0f);

		float direction_new_position = Vec3::dot(
				(new_position - current_wall.a) * Vec3(1.0f, 1.0f),
				ortho_direction_wall
				);

		float direction_old_position = Vec3::dot(
				(camera_position - current_wall.a) * Vec3(1.0f, 1.0f),
				ortho_direction_wall
				);

		/* The camera has gone through the portal if their directions are opposite.
		 * This is only possible to know if the sector is convex, it's quite important
		 * for the sector to be convex!!!! */
		if(direction_old_position * direction_new_position <= 0){
			camera_sector = current_wall.portal;
			break;
		}
	}

	camera_position = new_position;
}

void RenderPortal3d::render(PortalWorldComponent &world){
	Context *context = scene->getGame()->getContext();

	/* clear the current values */
	clearValuesY();
	clearDepthBuffer();
	framebuffer_canvas->clear(Color(0x00, 0x00, 0x00));

	/* compute current direction */
	camera_direction = Vec3(0.0f, 1.0f, 0.0f).rotateZ(camera_angle);

	updateCameraPosition(world, camera_position + camera_direction * context->getDeltaTime() * 0.25f);

	/* render sector from the camera sector */
	renderSector(
			world,
			camera_sector,
			0,
			framebuffer_width,
			std::unordered_set<int>()
			);

	/* update the texture which has the canvas */
	framebuffer_texture->updateCanvas(context, framebuffer_canvas);
}

void RenderPortal3d::renderSector(PortalWorldComponent& world, int sector_id, int min_x, int max_x, std::unordered_set<int> sectors_rendered){
	sectors_rendered.insert(sector_id);
	Sector& sector = world.sectors[sector_id];

	for(Wall current_wall : sector.walls){
		/* if the wall isn't on direction of the camera */
		if(!isWallOnCameraDirection(current_wall))
			continue;

		/* render normal wall */
		if(current_wall.portal == -1){
			/* check if at least one of the vertices is in front of the camera */
			if(maxDistanceFromCamera(current_wall) < CLIP_DISTANCE)
				continue;

			/* clip wall if necessary */
			Wall clipped_wall = clipWall(current_wall);

			/* check its visibility on camera plane */
			if(isWallVisible(clipped_wall, min_x, max_x))
				renderWall(world, sector, clipped_wall, current_wall.a, min_x, max_x, WALL_NORMAL);
		}
		else if(sectors_rendered.find(current_wall.portal) == sectors_rendered.end()){
			/* if the portal is a sector and if its sector hasn't been rendered yet... */
			float new_min_x = min_x, new_max_x = max_x;

			/* if the portal window is in front of the cliping distance...
			 * There's a bug that if you get too close, the wall will clip in
			 * a strange way, and the screen will go black, but i don't know
			 * how to solve this. */
			if(maxDistanceFromCamera(current_wall) >= CLIP_DISTANCE){
				Wall clipped_wall = clipWall(current_wall);
				renderPortalFrame(world, sector, clipped_wall, current_wall.a, min_x, max_x);

				new_min_x = getProjectionX(
						getProjectionOrtho(clipped_wall.a),
						getProjectionDistance(clipped_wall.a)
						);

				new_max_x = getProjectionX(
						getProjectionOrtho(clipped_wall.b),
						getProjectionDistance(clipped_wall.b)
						);

				if(new_min_x < min_x)
					new_min_x = min_x;

				if(new_max_x > max_x)
					new_max_x = max_x;
			}

			/* Render the next possible sector */
			renderSector(world, current_wall.portal, new_min_x, new_max_x, sectors_rendered);
		}
	}
}

void RenderPortal3d::renderWall(PortalWorldComponent& world, Sector& sector, Wall& wall, Vec3& start, int min_x, int max_x, int type){
	float top_left, bottom_left, top_right, bottom_right, distance_left, distance_right;

	distance_left = getProjectionDistance(wall.a);
	distance_right = getProjectionDistance(wall.b);

	top_left = getProjectionY(sector.celling_height, distance_left);
	bottom_left = getProjectionY(sector.floor_height, distance_left);

	top_right = getProjectionY(sector.celling_height, distance_right);
	bottom_right = getProjectionY(sector.floor_height, distance_right);

	float start_x = getProjectionX(getProjectionOrtho(wall.a), distance_left);
	float end_x = getProjectionX(getProjectionOrtho(wall.b), distance_right);

	Vec3 uv_start = Vec3((wall.a - start).length() / distance_left, 0.0f, 1.0f / distance_left);
	Vec3 uv_end = Vec3((wall.b - start).length() / distance_right, 0.0f, 1.0f / distance_right);

	for(int i = roundf(start_x); i < roundf(end_x); i++){
		if(i < min_x){
			i = min_x - 1;
			continue;
		}

		if(i >= max_x){
			break;
		}

		/* interpolating constant */
		float alpha = (i - start_x) / (end_x - start_x);

		float bottom = (bottom_right - bottom_left) * alpha + bottom_left;
		float top = (top_right - top_left) * alpha + top_left;

		if(bottom >= framebuffer_height)
			continue;

		if(top <= 0)
			continue;

		/* interpolate uv map */
		Vec3 uv = (uv_end - uv_start) * alpha + uv_start;
		float depth = 1.0f / uv.z;
		uv.x *= depth;

		/* drawing wall */
		for(int j = roundf(bottom); j < roundf(top); j++){
			Vec3 uv_with_scale = Vec3(uv.x, 0.0f);
			uv_with_scale.y = (j - bottom) / (top - bottom) * (sector.celling_height - sector.floor_height);

			uv_with_scale = uv_with_scale * wall.texture.scaling + wall.texture.offset;

			Color tile_color = getTileColor(wall.texture.id, uv_with_scale);
			putPixel(tile_color, i, j);
			setDepthBuffer(depth, i, j);
		}

		/* draw the bottom plane */
		if(type == WALL_NORMAL || type == WALL_PORTAL_BOTTOM){
			renderPlane(
					sector.floor_texture,
					sector.floor_height,
					i,
					bottom_value_y[i],
					bottom
					);

			if(top > bottom_value_y[i])
				bottom_value_y[i] = top;
		}

		/* draw the top plane */
		if(type == WALL_NORMAL || type == WALL_PORTAL_TOP){
			renderPlane(
					sector.celling_texture,
					sector.celling_height,
					i,
					top,
					top_value_y[i]
					);

			if(bottom < top_value_y[i])
				top_value_y[i] = bottom;
		}
	}
}

void RenderPortal3d::renderPortalFrame(PortalWorldComponent& world, Sector& sector, Wall& wall, Vec3& start, int min_x, int max_x){
	Sector sector_bottom, sector_top;
	Sector& portal_sector = world.sectors[wall.portal];

	/* create a "fake" sector for rendering the bottom part */
	sector_bottom.floor_texture = sector.floor_texture;
	sector_bottom.celling_texture = sector.celling_texture;
	sector_bottom.floor_height = sector.floor_height;
	sector_bottom.celling_height = portal_sector.floor_height;

	/* then create another "fake" sector for rendering the top part */
	sector_top.floor_texture = sector.floor_texture;
	sector_top.celling_texture = sector.celling_texture;
	sector_top.floor_height = portal_sector.celling_height;
	sector_top.celling_height = sector.celling_height;

	/* render both of them */
	renderWall(world, sector_bottom, wall, start, min_x, max_x, WALL_PORTAL_BOTTOM);
	renderWall(world, sector_top, wall, start, min_x, max_x, WALL_PORTAL_TOP);
}

void RenderPortal3d::renderPlane(TextureConfiguration& config, float height, float x, float start_y, float end_y){
	for(float j = roundf(start_y); j < roundf(end_y); j++){
		Vec3 uv = getVec3Inverse(height, x, j) * config.scaling + config.offset;

		Color tile_color = getTileColor(config.id, uv);
		float depth = getProjectionDistance(uv);

		putPixel(tile_color, x, j);
		setDepthBuffer(depth, x, j);
	}
}

bool RenderPortal3d::isWallOnCameraDirection(Wall &wall){
	float distance_a = getProjectionDistance(wall.a);
	float distance_b = getProjectionDistance(wall.b);

	return distance_a >= 0.0f || distance_b >= 0.0f;
}

float RenderPortal3d::minDistanceFromCamera(Wall &wall){
	float distance_a = getProjectionDistance(wall.a);
	float distance_b = getProjectionDistance(wall.b);

	if(distance_a < distance_b)
		return distance_a;

	return distance_b;
}

float RenderPortal3d::maxDistanceFromCamera(Wall &wall){
	float distance_a = getProjectionDistance(wall.a);
	float distance_b = getProjectionDistance(wall.b);

	if(distance_a > distance_b)
		return distance_a;

	return distance_b;
}

bool RenderPortal3d::isWallVisible(Wall &wall, int min_x, int max_x){
	float distance_a = getProjectionDistance(wall.a);
	float distance_b = getProjectionDistance(wall.b);

	float x_a = getProjectionX(getProjectionOrtho(wall.a), distance_a);
	float x_b = getProjectionX(getProjectionOrtho(wall.b), distance_b);

	if(fmaxf(x_a, x_b) < min_x) return false;
	if(fminf(x_a, x_b) >= max_x) return false;

	return true;
}

Wall RenderPortal3d::clipWall(Wall &wall){
	Wall copy = wall;

	float distance_a = getProjectionDistance(copy.a);
	float distance_b = getProjectionDistance(copy.b);

	if(getProjectionOrtho(copy.a) * distance_b > getProjectionOrtho(copy.b) * distance_a){
		std::swap(copy.a, copy.b);
		std::swap(distance_a, distance_b);
	}

	if(distance_a >= CLIP_DISTANCE && distance_b >= CLIP_DISTANCE)
		return copy;

	if(distance_a < CLIP_DISTANCE && distance_b > CLIP_DISTANCE){
		copy.a = (copy.b - copy.a) * (CLIP_DISTANCE - distance_a) / (distance_b - distance_a) + copy.a;

		return copy;
	}

	if(distance_b < CLIP_DISTANCE && distance_a > CLIP_DISTANCE){
		copy.b = (copy.a - copy.b) * (CLIP_DISTANCE - distance_b) / (distance_a - distance_b) + copy.b;

		return copy;
	}

	return copy;
}

float RenderPortal3d::getProjectionDistance(Vec3 point){
	return Vec3::dot((point - camera_position) * Vec3(1.0f, 1.0f, 0.0f), camera_direction);
}

float RenderPortal3d::getProjectionOrtho(Vec3 point){
	return Vec3::dot((point - camera_position) * Vec3(1.0f, 1.0f, 0.0f), Vec3(camera_direction.y, -camera_direction.x, 0.0f));
}

float RenderPortal3d::getProjectionX(float projection_x, float distance){
	return
		((projection_x) / distance * 0.5f) * framebuffer_canvas->getHeight() + framebuffer_canvas->getWidth() / 2;
}

float RenderPortal3d::getProjectionY(float height, float distance){
	return
		((height - camera_position.z) / distance * 0.5f + 0.5f) * framebuffer_canvas->getHeight();
}

Vec3 RenderPortal3d::getVec3Inverse(float height, float i, float j){
	if(i < 0 || j < 0 || i >= framebuffer_width || j >= framebuffer_height) return Vec3();

	float y = (height - camera_position.z) * div_table_y[(int) j];
	float x = y * div_table_x[(int) i];

	Vec3 position = camera_direction * y + Vec3(camera_direction.y, -camera_direction.x, 0.0f) * x;

	position += camera_position;

	position.z = 0.0f;

	return position;
}

void RenderPortal3d::putPixel(Color color, int i, int j){
	framebuffer_canvas->setColor(color, i, framebuffer_canvas->getHeight() - j);
}


void RenderPortal3d::setDepthBuffer(float depth, int i, int j){
	if(i < 0 || j < 0 || i >= framebuffer_width || j >= framebuffer_height)
		return;

	int index = i + j * framebuffer_width;

	depth_buffer[index] = depth;
}

float RenderPortal3d::getDepthBuffer(int i, int j){
	if(i < 0 || j < 0 || i >= framebuffer_width || j >= framebuffer_height)
		return 0.0f;

	int index = i + j * framebuffer_width;

	return depth_buffer[index];
}

Color RenderPortal3d::getTileColor(int id, Vec3 uv){
	int num_tex_x = tilemap_canvas->getWidth() / tile_width;

	int id_x = (id % num_tex_x);
	int id_y = (id / num_tex_x);

	uv.x = fmodf(uv.x, 1.0f);
	if(uv.x < 0.0f) uv.x += 1.0f;

	uv.y = fmodf(uv.y, 1.0f);
	if(uv.y < 0.0f) uv.y += 1.0f;

	int x = id_x * tile_width + uv.x * tile_width;
	int y = id_y * tile_height + uv.y * tile_height;

	return tilemap_canvas->getColor(x, y);
}

};
