#include "Systems/RenderPortal3d.hpp"
#include "Game.hpp"

#define CLIP_DISTANCE 1.0f

namespace acc {

RenderPortal3d::RenderPortal3d(ScenePortal3d *scene){
	this->scene = scene;

	framebuffer_canvas = scene->getFramebufferCanvas();
	framebuffer_texture = scene->getFramebufferTexture();
	tilemap_canvas = (Canvas *) scene->getGame()->getResource("tile_canvas.png");

	tile_width = tile_height = 64;

	camera_position = Vec3(0.0f, 0.0f, 0.0f);
	camera_angle = 0.0f;
}

void RenderPortal3d::update(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<PortalWorldComponent>())
		return;

	auto arr = component_manager->getComponentArray<PortalWorldComponent>();

	for(size_t i = 0; i < arr->getSize(); i++)
		render(arr->atIndex(i));
}

void RenderPortal3d::render(PortalWorldComponent &world){
	Context *context = scene->getGame()->getContext();

	framebuffer_canvas->clear(Color(0x00, 0x00, 0x00));
	
	camera_angle -= context->getDeltaTime() / 8;

	camera_direction = Vec3(0.0f, 1.0f, 0.0f).rotateZ(camera_angle);

	Sector sector;

	sector.floor_height = -0.5f;
	sector.celling_height = +0.5f;

	Wall wall;
	wall.a = Vec3(-1.0f, 4.0f, 0.0f);
	wall.b = Vec3(+1.0f, 6.0f, 0.0f);
	
	sector.walls.push_back(wall);

	wall.a = Vec3(+1.0f, 6.0f, 0.0f);
	wall.b = Vec3(+3.0f, 4.0f, 0.0f);

	sector.walls.push_back(wall);

	renderSector(sector);

	framebuffer_texture->updateCanvas(scene->getGame()->getContext(), framebuffer_canvas);
}

void RenderPortal3d::renderSector(Sector& sector){
	printf("start\n");

	for(Wall i : sector.walls){
		if(!isWallVisible(i))
			continue;

		Wall j = clipWall(i);

		renderWall(sector, j, i.a);
	}
}

void RenderPortal3d::renderWall(Sector& sector, Wall& wall, Vec3& start){
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
		if(i < 0){
			i = 0;
			continue;
		}

		if(i > framebuffer_canvas->getWidth()){
			break;
		}

		float bottom = (bottom_right - bottom_left) * (i - start_x) / (end_x - start_x) + bottom_left;
		float top = (top_right - top_left) * (i - start_x) / (end_x - start_x) + top_left;
		Vec3 uv = (uv_end - uv_start) * (i - start_x) / (end_x - start_x) + uv_start;

		uv.x /= uv.z;

		for(int j = roundf(bottom); j < roundf(top); j++){
			uv.y = (j - bottom) / (top - bottom);
			Color c = getTileColor(1, uv);
			putPixel(c, i, j);
		}
	}
}

bool RenderPortal3d::isWallVisible(Wall &wall){
	float distance_a = getProjectionDistance(wall.a);
	float distance_b = getProjectionDistance(wall.b);

	float ortho_a = getProjectionOrtho(wall.a);
	float ortho_b = getProjectionOrtho(wall.b);

	return distance_a >= CLIP_DISTANCE || distance_b >= CLIP_DISTANCE;
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

	if(distance_a < CLIP_DISTANCE){
		copy.a = (copy.b - copy.a) * (CLIP_DISTANCE - distance_a) / (distance_b - distance_a) + copy.a;

		return copy;
	}

	if(distance_b < CLIP_DISTANCE){
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
		((projection_x) / distance) * framebuffer_canvas->getHeight() + framebuffer_canvas->getWidth() / 2;
}

float RenderPortal3d::getProjectionY(float height, float distance){
	return
		((height - camera_position.z) / distance + 0.5f) * framebuffer_canvas->getHeight();
}

void RenderPortal3d::putPixel(Color color, int i, int j){
	framebuffer_canvas->setColor(color, i, framebuffer_canvas->getHeight() - j);
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
