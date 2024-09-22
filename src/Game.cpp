#include "Context.hpp"
#include "Game.hpp"
#include "Scene.hpp"

#include "Texture.hpp"
#include "Font.hpp"

#include <cmath>

namespace acc {

float ang = 0;

Game::Game(const char *title, int internal_width, int internal_height){
	context = new Context(title, internal_width, internal_height);
	resource_manager = new ResourceManager();
	current_scene = NULL;
	next_scene = NULL;

	loadResource((Resource *) new Texture(context, "player.png", 32, 32));
	loadResource((Resource *) new Texture(context, "texture.png", 16, 32));
	loadResource((Resource *) new Texture(context, DEV_TEXTURE_WHITE));
	loadResource((Resource *) new Texture(context, DEV_TEXTURE_XOR));
	loadResource((Resource *) new Font("default.ttf", 12));
}

void Game::setScene(Scene *scene){
	this->next_scene = scene;
}

void Game::loadResource(Resource *resource){
	resource_manager->loadResource(resource);
}

Resource * Game::getResource(std::string filename){
	return resource_manager->getResource(filename);
}

void Game::update(void){
	context->clearScreen(100, 100, 100, 255);

	if(current_scene != NULL){
		current_scene->update();
	}

	Texture *texture = (Texture *) getResource("dev_texture_xor");

	Triangle my_triangle(
			Vertex(Vec3(0.0f, 0.0f, 100.0f), Vec3(0.0f, 0.0f)),
			Vertex(Vec3(100.0f, 0.0f, -100.0f), Vec3(1.0f, 0.0f)),
			Vertex(Vec3(200.0f, 0.0f, 30.0f), Vec3(0.0f, 1.0f))
			);

	std::vector<Triangle> triangle_list;

	//triangle_list.push_back(my_triangle);
	my_triangle.clipOverZ(&triangle_list);
	//my_triangle.subdivideForUVNormal(&triangle_list);

	//printf("%ld\n", triangle_list.size());

	size_t index = 0;

	for(auto& t : triangle_list){
		index++;
		index %= 4;
		t.applyTransformation(
				[index](Vertex a){
					//a.uv *= Vec3(1.0f / 16.0f, 1.0f / 32.0f);
					//a.position -= Vec3(100.0f, 100.0f);
					//a.position = a.position.rotateZ(ang);
					//a.position = a.position.rotateY(ang);
					//a.position += Vec3(100.0f, 100.0f);

					a.position = Vec3(a.position.x, a.position.z, a.position.y);
					a.position += Vec3(0, 100, 0);
					//a.position.print();
					//a.uv.print();

					Vec3 tst[4] = {
						Vec3(1.0f, 0.0f, 0.0f),
						Vec3(0.0f, 1.0f, 0.0f), 
						Vec3(0.0f, 0.0f, 1.0f),
						Vec3(1.0f, 1.0f, 1.0f)
						};
					//a.color = tst[index];

					return a;
				}
				);
		texture->renderTriangle(context, t);
	}

	ang += context->getDeltaTime();

	context->renderPresent();
}

void Game::loop(void){
	context->pollEvent();

	if(next_scene != NULL){
		if(current_scene != NULL)
			delete current_scene;

		current_scene = next_scene;
		next_scene = NULL;
	}

	update();

	context->updateTime();
}

void Game::run(void){
	while(context->isRunning()){
		loop();
	}
}

Context * Game::getContext(void){
	return context;
}

Game::~Game(void){
	if(current_scene != NULL)
		delete current_scene;

	if(next_scene != NULL)
		delete next_scene;

	delete resource_manager;
	delete context;
}

};
