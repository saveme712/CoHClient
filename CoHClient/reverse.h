#pragma once
#include <cstdint>
#include <glm/glm.hpp>

#define MAX_ENTITIES 0x2800

#pragma pack(push, 1)
typedef struct _Entity
{
	// 0x0
	char* name;
	// 0x8
	char padding_000[0x30];
	// 0x38
	void* seq;
	// 0x40
	char padding_01[0x14];
	// 0x54
	glm::mat4 fork;
} Entity;
static_assert(offsetof(Entity, fork) == 0x54, "Invalid offset");

#pragma pack(pop)

typedef struct RdrViewState
{
	glm::vec4 sun_ambient;
	glm::vec4 sun_diffuse;
	glm::vec4 sun_diffuse_reverse;
	glm::vec4 sun_ambient_for_players;
	glm::vec4 sun_diffuse_for_players;
	glm::vec4 sun_diffuse_reverse_for_players;
	glm::vec4 sun_direction;
	glm::vec4 sun_direction_in_viewspace;
	glm::vec4 sun_no_angle_light;
	glm::vec3 shadow_direction;
	glm::vec3 shadowmap_light_direction;
	glm::vec3 shadowmap_light_direction_in_viewspace;
	float shadowmap_extrusion_scale;
	float lowest_visible_point_last_frame;
	glm::vec3 sun_position;
	glm::vec4 shadowcolor;
	glm::mat4 viewmat;
	glm::mat4 inv_viewmat;
	float nearz, farz;
	glm::mat4x4 projectionMatrix;
	glm::mat4x4 projectionClipMatrix; // sheared version of projection to do a user clip plane (e.g. reflection clipping)
	float gloss_scale;
	uint32_t gfxdebug;
	uint32_t wireframe : 3;
	uint32_t shadowvol : 3;
	uint32_t wireframe_seams : 1;
	uint32_t force_double_sided : 1;
	uint32_t write_depth_only : 1;
	uint32_t fixed_sun_pos : 1;
	int width, height; // window size
	int origWidth, origHeight; // Saved values if pbuffers changed them
	bool override_window_size;
	float client_loop_timer;
	float time;
	float lamp_alpha;
	float toneMapWeight;
	float bloomWeight;
	float luminance;
	float desaturateWeight;
	uint8_t bloomScale;
	uint8_t lamp_alpha_byte;
	int white_tex_id, dummy_bump_tex_id, grey_tex_id, black_tex_id, building_lights_tex_id;
	int texAnisotropic;
	int antialiasing;
	int renderScaleFilter;
	glm::vec3 fogcolor;
	glm::vec2 fogdist;
	float dofFocusDistance;
	float dofFocusValue;
	float dofNearDist;
	float dofNearValue;
	float dofFarDist;
	float dofFarValue;
	float base_reflectivity;
	int32_t renderPass;

	int cubemapMode;
	int shadowMode;

	float waterRefractionSkewNormalScale;
	float waterRefractionSkewDepthScale;
	float waterRefractionSkewMinimum;
	int waterReflectionOverride;
	float waterReflectionSkewScale;
	float waterReflectionStrength;
	float waterFresnelBias;
	float waterFresnelScale;
	float waterFresnelPower;
	float waterTODscale;

} RdrViewState;
