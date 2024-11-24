#include "Systems/Render3d.hpp"
#include "Components/Sprite3d.hpp"
#include "Components/Object3dComponent.hpp"
#include "Components/World3dComponent.hpp"
#include "Components/MovBrush3dComponent.hpp"
#include "Components/Components.hpp"
#include "RendererGL.hpp"

#include <algorithm>

static const float pi = 3.141592f;

namespace acc {

Render3dSystem::Render3dSystem(Context *context, Mat4 *view){
	this->view = view;
	this->context = context;

	this->sprite_mesh = new Mesh();
	this->sprite_mesh->buildUnitQuad();
	this->cube_mesh = new Mesh();
	this->cube_mesh->buildUnitCube();

	projection = Mat4::PerspectiveProjection(
			(float) context->getWidth() / context->getHeight(),
			pi * 90.0f / 180.0f,
			100.0f,
			0.1f
			);

}

void Render3dSystem::update(ComponentManager *component_manager){
	RendererGL *gl = (RendererGL *) context->getRenderer();

	gl->setDepthBuffer(true);

	updateWorld(component_manager);
	updateMovBrush(component_manager);
	updateObjects(component_manager);

	gl->setDepthMask(false);
	updateSprites(component_manager);
	gl->setDepthMask(true);

	gl->setDepthBuffer(false);
}

void Render3dSystem::updateSprites(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<Sprite3dComponent>())
		return;

	auto sprite_array = component_manager->getComponentArray<Sprite3dComponent>();

	std::vector<Sprite3dComponent *> sprites_to_render;
	sprites_to_render.reserve(sprite_array->getSize());

	for(size_t i = 0; i < sprite_array->getSize(); i++){
		Entity entity = sprite_array->indexToEntity(i);
		auto& current_sprite = sprite_array->atIndex(i);

		if(current_sprite.texture == NULL)
			continue;

		if(!component_manager->hasComponent<TransformComponent>(entity))
			continue;

		auto& current_transform = component_manager->getComponent<TransformComponent>(entity);
		current_sprite.position = current_transform.position;

		current_sprite.model = Mat4::Transform(current_sprite.position) * current_sprite.rotation * current_sprite.scale;

		sprites_to_render.push_back(&current_sprite);
	}

	auto func_cmp = [this](Sprite3dComponent *a, Sprite3dComponent *b){
		Vec3 in_a = (*(this->view)) * a->position;
		Vec3 in_b = (*(this->view)) * b->position;

		return in_a.z < in_b.z;
	};

	std::sort(sprites_to_render.begin(), sprites_to_render.end(), func_cmp);

	for(Sprite3dComponent *current_sprite : sprites_to_render){
		current_sprite->shader->setUniform("model", current_sprite->model);
		current_sprite->shader->setUniform("view", *view);
		current_sprite->shader->setUniform("projection", projection);
		current_sprite->shader->setTexture("sprite_texture", current_sprite->texture->getId(), 0);

		float ix, iy, iw, ih;
		current_sprite->texture->getIdMap(
				current_sprite->id,
				current_sprite->flip_horizontally,
				current_sprite->flip_vertically,
				&ix,
				&iy,
				&iw,
				&ih
				);

		current_sprite->shader->setUniform("mapping", ix, iy, iw, ih);
		sprite_mesh->render(current_sprite->shader);
	}
}

void Render3dSystem::updateObjects(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<Object3dComponent>())
		return;

	auto brush_array = component_manager->getComponentArray<Object3dComponent>();

	for(size_t i = 0; i < brush_array->getSize(); i++){
		auto& current_brush = brush_array->atIndex(i);

		current_brush.shader->setUniform("model", current_brush.model);
		current_brush.shader->setUniform("projection", projection);
		current_brush.shader->setUniform("view", *view);

		current_brush.model3d->render(current_brush.shader);
	}
}

void Render3dSystem::updateWorld(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<World3dComponent>())
		return;

	auto world_array = component_manager->getComponentArray<World3dComponent>();

	for(size_t i = 0; i < world_array->getSize(); i++){
		auto& current_world = world_array->atIndex(i);

		if(current_world.debug_render_octree){
			current_world.octree_shader->setUniform("projection", projection);
			current_world.octree_shader->setUniform("view", *view);

			for(OctreeNode *node : current_world.octree->getNodes()){
				if(node->brushes.size() == 0)
					continue;

				current_world.octree_shader->setUniform("model", Mat4::Transform(node->start) * Mat4::Scale(node->size) * Mat4::Transform(0.5f, 0.5f, 0.5f));
				
				cube_mesh->render(current_world.octree_shader);
			}
		}

		current_world.shader->setUniform("model", Mat4::Identity());
		current_world.shader->setUniform("projection", projection);
		current_world.shader->setUniform("view", *view);

		for(BrushBuilder *i : current_world.brushes){
			Model3d *model = i->getModel();

			if(i == NULL)
				continue;

			model->render(current_world.shader);
		}
	}
}

void Render3dSystem::updateMovBrush(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<MovBrush3dComponent>())
		return;

	auto brush_array = component_manager->getComponentArray<MovBrush3dComponent>();

	for(size_t i = 0; i < brush_array->getSize(); i++){
		Entity entity = brush_array->indexToEntity(i);
		auto& current_brush = brush_array->atIndex(i);
		Model3d *model = current_brush.brush->getModel();

		if(model == NULL)
			continue;

		if(!component_manager->hasComponent<TransformComponent>(entity))
			continue;

		auto& transform = component_manager->getComponent<TransformComponent>(entity);
		current_brush.position = transform.position;

		current_brush.shader->setUniform("model", Mat4::Transform(current_brush.position));
		current_brush.shader->setUniform("projection", projection);
		current_brush.shader->setUniform("view", *view);

		model->render(current_brush.shader);
	}
}

};
