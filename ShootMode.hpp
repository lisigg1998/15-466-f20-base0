#include "ColorTextureProgram.hpp"

#include "Mode.hpp"
#include "GL.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <deque>

/*
 * ShootMode is a game mode that implements a single-player game of Pong.
 */

struct ShootMode : Mode {
	ShootMode();
	virtual ~ShootMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const&, glm::uvec2 const& window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const& drawable_size) override;
	glm::mat4 get_rotation_matrix(glm::vec2 const& center, float const& angle);

	//----- game state -----

	glm::vec2 court_radius = glm::vec2(7.0f, 5.0f);
	glm::vec2 paddle_radius = glm::vec2(0.2f, 1.0f);
	glm::vec2 ball_radius = glm::vec2(0.2f, 0.2f);

	glm::vec2 left_paddle = glm::vec2(0.0f, -3.0f);
	glm::vec2 right_paddle = glm::vec2(court_radius.x - 0.5f, 0.0f);

	glm::vec2 ball = glm::vec2(-court_radius.x + ball_radius[0] + FLT_EPSILON, 0.0f);
	glm::vec2 ball_velocity = glm::vec2(0.0f, 0.0f);

	glm::vec2 cannon_base = glm::vec2(-court_radius.x, 0.0f);
	glm::vec2 cannon_base_radius = glm::vec2(0.8f, 0.8f);

	glm::vec2 cannon_barrel = cannon_base;
	glm::vec2 cannon_barrel_length = glm::vec2(1.8f, 1.8f); 
	glm::vec2 cannon_barrel_radius = glm::vec2(0.25f, 0.25f);
	glm::vec3 cannon_rotate_axis = glm::vec3(0.0f,0.0f, 1.0f);
	float cannon_angle = 0;  //in radians
	float max_cannon_elevation = glm::radians(70.0f);  //in radians
	float barrel_offset = 0.5f;

	uint32_t max_score = 10;
	uint32_t left_score = 0;
	uint32_t max_health = 3;
	uint32_t left_health = max_health;

	float left_ai_offset = 0.0f;
	float right_ai_offset = 0.0f;
	float left_ai_offset_update = 0.0f;
	float right_ai_offset_update = 0.0f;

	uint32_t pause_flag = 0;
	uint32_t shoot_flag = 0;
	uint32_t game_flag = 0; //1:game over; 2:win

	//----- pretty gradient trails -----

	float trail_length = 0.0f;
	std::deque< glm::vec3 > ball_trail; //stores (x,y,age), oldest elements first

	//----- opengl assets / helpers ------

	//draw functions will work on vectors of vertices, defined as follows:
	struct Vertex {
		Vertex(glm::vec3 const& Position_, glm::u8vec4 const& Color_, glm::vec2 const& TexCoord_) :
			Position(Position_), Color(Color_), TexCoord(TexCoord_) { }
		glm::vec3 Position;
		glm::u8vec4 Color;
		glm::vec2 TexCoord;
	};
	static_assert(sizeof(Vertex) == 4 * 3 + 1 * 4 + 4 * 2, "ShootMode::Vertex should be packed");

	//Shader program that draws transformed, vertices tinted with vertex colors:
	ColorTextureProgram color_texture_program;

	//Buffer used to hold vertex data during drawing:
	GLuint vertex_buffer = 0;

	//Vertex Array Object that maps buffer locations to color_texture_program attribute locations:
	GLuint vertex_buffer_for_color_texture_program = 0;

	//Solid white texture:
	GLuint white_tex = 0;

	//matrix that maps from clip coordinates to court-space coordinates:
	glm::mat3x2 clip_to_court = glm::mat3x2(1.0f);
	// computed in draw() as the inverse of OBJECT_TO_CLIP
	// (stored here so that the mouse handling code can use it to position the paddle)

};
