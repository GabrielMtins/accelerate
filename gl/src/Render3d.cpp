#include "Systems/Render3d.hpp"
#include "Components/Sprite3d.hpp"
#include "Components/Brush3dComponent.hpp"
#include "Components/Components.hpp"
#include "RendererGL.hpp"

const float pi = 3.141592;

namespace acc {

Render3dSystem::Render3dSystem(Context *context, Mat4 *view){
	this->view = view;
	this->context = context;

	this->sprite_mesh = new Mesh();
	this->sprite_mesh->buildUnitQuad();

	projection = Mat4::PerspectiveProjection(
			(float) context->getWidth() / context->getHeight(),
			pi / 3,
			100.0f,
			0.1f
			);

}

void Render3dSystem::update(ComponentManager *component_manager){
	RendererGL *gl = (RendererGL *) context->getRenderer();

	gl->setDepthBuffer(true);

	updateBrushes(component_manager);
	updateSprites(component_manager);

	gl->setDepthBuffer(false);
}

void Render3dSystem::updateSprites(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<Sprite3dComponent>())
		return;

	auto sprite_array = component_manager->getComponentArray<Sprite3dComponent>();

	for(size_t i = 0; i < sprite_array->getSize(); i++){
		Entity entity = sprite_array->indexToEntity(i);
		auto& current_sprite = sprite_array->atIndex(i);

		if(current_sprite.texture == NULL)
			continue;

		if(!component_manager->hasComponent<TransformComponent>(entity))
			continue;

		auto& current_transform = component_manager->getComponent<TransformComponent>(entity);
		current_sprite.position = current_transform.position;

		Mat4 model = Mat4::Identity();

		model = Mat4::Transform(current_sprite.position) * current_sprite.rotation * current_sprite.scale * model;

		current_sprite.shader->setUniform("model", model);
		current_sprite.shader->setUniform("projection", projection);
		current_sprite.shader->setUniform("view", *view);
		current_sprite.shader->setTexture("sprite_texture", current_sprite.texture->getId(), 0);

		float ix, iy, iw, ih;
		current_sprite.texture->getIdMap(
				current_sprite.id,
				current_sprite.flip_horizontally,
				current_sprite.flip_vertically,
				&ix,
				&iy,
				&iw,
				&ih
				);

		current_sprite.shader->setUniform("mapping", ix, iy, iw, ih);
		sprite_mesh->render(current_sprite.shader);
	}
}

void Render3dSystem::updateBrushes(ComponentManager *component_manager){
	if(!component_manager->hasComponentArray<Brush3dComponent>())
		return;

	auto brush_array = component_manager->getComponentArray<Brush3dComponent>();

	for(size_t i = 0; i < brush_array->getSize(); i++){
		auto& current_brush = brush_array->atIndex(i);

		current_brush.shader->setUniform("model", current_brush.model);
		current_brush.shader->setUniform("projection", projection);
		current_brush.shader->setUniform("view", *view);

		current_brush.model3d->render(current_brush.shader);
	}
}

};
