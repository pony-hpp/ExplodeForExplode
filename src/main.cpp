#include "core/exception.hpp"
#include "core/logger.hpp"
#include "core/rand.hpp"
#include "core/renderer.hpp"
#include "core/window.hpp"
#include "game/dynamites/dynamite.hpp"
#include "game/movement.hpp"
#include "game/world/structures/structure_id.hpp"
#include "game/world/world.hpp"
#include "game/world_generators/plain_world_generator.hpp"
#include "game/zooming.hpp"
#include "opengl/shading/shader_program.hpp"
#include "opengl/shading/uniforms.hpp"

#include <memory>
#include <thread>
#include <vector>

static constexpr float _MV_SENSITIVITY = 1.5f;

class Game final
{
public:
  Game() noexcept
    : _logger("Game"), _mv(_MV_SENSITIVITY), _zoom(0.15f, 0.5f, 5.5f)
  {
    _logger.info_fmt("Welcome to Explode for Explode %s!", EFE_VERSION);
  }

  ~Game() noexcept { _logger.info_fmt("Goodbye!"); }

  bool setup_win() noexcept
  {
    _win = std::make_unique<core::Window>(1024, 768, "Explode for Explode");

    try
    {
      _win->create();
    }
    catch (const core::WindowCreationException &e)
    {
      _logger.critical_fmt(
        "The window couldn't be created: %s.", e.msg.c_str()
      );
      return false;
    };

    try
    {
      _win->set_icon(_pngDecoder("../assets/textures/ui/window_icon.png"));
    }
    catch (const core::FopenException &)
    {
      _logger.error_fmt("Failed to open window icon image.");
    }
    catch (const core::CorruptedPngException &)
    {
      _logger.error_fmt("Window icon image is corrupted.");
    }

    _win->set_bg(165, 190, 251);

    return true;
  }

  bool setup_renderer() noexcept
  {
    _renderer = std::make_unique<core::Renderer>(*_win);
    try
    {
      _renderer->init();
      return true;
    }
    catch (const core::RendererInitializationException &e)
    {
      _logger.critical_fmt(
        "Renderer couldn't be initialized: %s.", e.msg.c_str()
      );
      return false;
    }
  }

  bool compile_shaders() noexcept
  {
    _worldShaderProgram    = std::make_unique<gl::ShaderProgram>("World");
    _dynamiteShaderProgram = std::make_unique<gl::ShaderProgram>("Dynamite");
    _pondShaderProgram     = std::make_unique<gl::ShaderProgram>("Pond");

    try
    {
      { // World shader program.
        gl::VertexShader vs;
        vs.load("../assets/shaders/world.vert");
        vs.compile();
        _worldShaderProgram->add(vs);
        gl::FragmentShader fs;
        fs.load("../assets/shaders/world.frag");
        fs.compile();
        _worldShaderProgram->add(fs);
        _worldShaderProgram->link();
      }
      { // Dynamite shader program.
        gl::VertexShader vs;
        vs.load("../assets/shaders/dynamite.vert");
        vs.compile();
        _dynamiteShaderProgram->add(vs);
        gl::FragmentShader fs;
        fs.load("../assets/shaders/dynamite.frag");
        fs.compile();
        _dynamiteShaderProgram->add(fs);
        _dynamiteShaderProgram->link();
      }
      { // Pond shader program.
        gl::VertexShader vs;
        vs.load("../assets/shaders/pond.vert");
        vs.compile();
        _pondShaderProgram->add(vs);
        gl::FragmentShader fs;
        fs.load("../assets/shaders/pond.frag");
        fs.compile();
        _pondShaderProgram->add(fs);
        _pondShaderProgram->link();
      }
    }
    catch (...)
    {
      _logger.critical("The shaders couldn't be compiled.");
      return false;
    }

    _renderer->add_shader_program(*_worldShaderProgram);
    _renderer->add_shader_program(*_dynamiteShaderProgram);
    _renderer->add_shader_program(*_pondShaderProgram);

    game::Block::default_shader_program(*_worldShaderProgram);
    game::Block::match_shader_program(game::blocks::WATER, *_pondShaderProgram);
    game::Block::match_shader_program(
      game::blocks::WATER_WAVE, *_pondShaderProgram
    );

    return true;
  }

  void setup_movement_and_zooming() noexcept
  {
    _win->on_mouse_click(
      [this](core::mouse::Button btn, core::mouse::Action action) noexcept
    {
      if (btn == core::mouse::BUTTON_RIGHT)
      {
        _win->toggle_cursor_visibility();
        _rightBtnHeld = action == core::mouse::ACTION_PRESS;
        if (!_rightBtnHeld)
        {
          _mv.set_next_origin();
        }
      }
      else if (btn == core::mouse::BUTTON_LEFT &&
               action == core::mouse::ACTION_PRESS)
      {
        _create_dynamite();
      }
    }
    );

    _setup_movement();
    _setup_zooming();

    // Set origin.
    _mv(
      (_worldSettings.w / 2.0f) * game::Block::SIZE / _MV_SENSITIVITY -
        _win->w() / 2.0f,
      -(_worldSettings.h() * game::Block::SIZE - 15 * game::Block::SIZE) /
        _MV_SENSITIVITY
    );
    _mv(0, 0); // Apply position.
    _mv.set_next_origin();
    _renderer->view.set_offset(_mv.get().x, _mv.get().y);
    _renderer->update_view();
  }

  void create_world_settings() noexcept
  {
    _worldSettings.w = 200;
    _worldSettings.layers.push_back({game::blocks::GRASS_BLOCK, 1});
    _worldSettings.layers.push_back({game::blocks::EARTH_BLOCK, 34});
    _worldSettings.layers.push_back({game::blocks::STONE_BLOCK, 65});

    _worldShaderProgram->set_uniform(
      gl::UNIFORM_WORLD_H, _worldSettings.h() * game::Block::SIZE
    );

    const unsigned short
      kPondW = game::Structure::from_id(game::structures::POND)->w(),
      kPondH = game::Structure::from_id(game::structures::POND)->h();

    _pondShaderProgram->set_uniform(
      gl::UNIFORM_POND_TOP, (_worldSettings.h() - 1) * game::Block::SIZE
    );
    _pondShaderProgram->set_uniform(
      gl::UNIFORM_POND_BOTTOM,
      (_worldSettings.h() - 1 - kPondH) * game::Block::SIZE
    );

    for (unsigned i = 0; i < _worldSettings.w; i++)
    {
      if (core::rand(0, 15) == 0)
      {
        _worldSettings.structures.push_back(
          {game::structures::TREE, (int)i, _worldSettings.h()}
        );
      }
      else if (core::rand(0, 6) != 0)
      {
        _worldSettings.structures.push_back(
          {game::structures::GRASS, (int)i, _worldSettings.h()}
        );
      }
      else if (core::rand(0, 8) == 0 && i + kPondW <= _worldSettings.w)
      {
        _worldSettings.structures.push_back(
          {game::structures::POND, (int)i, _worldSettings.h() - 1}
        );
        i += kPondW;
      }
    }
  }

  void generate_world() noexcept
  {
    game::PlainWorldGenerator worldGen(_worldSettings);
    _world = std::make_unique<game::World>(*worldGen());
    _world->load_textures(_pngDecoder);
  }

  void operator()() noexcept
  {
    while (!_win->closed())
    {
      _win->poll_events();
      _win->clear();

      for (auto &dynamite : _dynamites)
      {
        if (dynamite.first && dynamite.second)
        {
          dynamite.first->explode();
          dynamite.first.reset();
        }
      }

      _renderer->draw(*_world);

      _dynamiteShaderProgram->use();
      for (const auto &dynamite : _dynamites)
      {
        if (dynamite.first)
        {
          _renderer->draw(*dynamite.first);
        }
      }

      _win->update();
    }
  }

private:
  std::unique_ptr<core::Window> _win;
  std::unique_ptr<core::Renderer> _renderer;
  std::unique_ptr<gl::ShaderProgram> _worldShaderProgram,
    _dynamiteShaderProgram, _pondShaderProgram;

  core::PngDecoder _pngDecoder;

  game::PlainWorldGeneratorSettings _worldSettings;
  std::unique_ptr<game::World> _world;

  game::Movement _mv;
  game::Zooming _zoom;
  bool _rightBtnHeld = false;

  unsigned _explodedDynamites = 0;
  std::vector<std::pair<std::unique_ptr<game::Dynamite>, bool>> _dynamites;

  core::Logger _logger;

  void _setup_movement() noexcept
  {
    _win->on_cursor_move([this](int x, int y) noexcept
    {
      if (_rightBtnHeld)
      {
        _mv(x / _zoom.get().scale, y / _zoom.get().scale);

        _renderer->view.set_offset(
          _zoom.get().offsetX + _mv.get().x * _zoom.get().scale,
          _zoom.get().offsetY + _mv.get().y * _zoom.get().scale
        );
        _renderer->update_view();
      }
    });
  }

  void _setup_zooming() noexcept
  {
    _win->on_scroll([this](bool up) noexcept
    {
      _zoom(up, _renderer->viewport_w() / 2.0f, _renderer->viewport_h() / 2.0f);

      _renderer->view.set_scale(_zoom.get().scale);
      _renderer->view.set_offset(
        _zoom.get().offsetX + _mv.get().x * _zoom.get().scale,
        _zoom.get().offsetY + _mv.get().y * _zoom.get().scale
      );
      _renderer->update_view();
    });
  }

  void _create_dynamite() noexcept
  {
    std::thread([this]() noexcept
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(750));

      if (!_win->closed()) // Avoid UB when window is closed.
      {
        _dynamites[_explodedDynamites++].second = true;
      }
    }).detach();

    _dynamites.push_back({std::make_unique<game::Dynamite>(*_world), false});

    _dynamites.back().first->load_texture(_pngDecoder);

    const float x = _zoom.get().offsetX / _zoom.get().scale + _mv.get().x +
                    _win->cursor_x() / _zoom.get().scale,
                y = _zoom.get().offsetY / _zoom.get().scale + _mv.get().y +
                    _win->cursor_y() / _zoom.get().scale;
    _dynamites.back().first->set_pos(x, y);

    _logger.info_fmt("Created dynamite at (%.1f; %.1f).", x, y);
  }
};

int main()
{
  Game game;

  if (!game.setup_win())
  {
    return 1;
  }

  if (!game.setup_renderer())
  {
    return 1;
  }

  if (!game.compile_shaders())
  {
    return 1;
  }

  game.create_world_settings();
  game.generate_world();

  game.setup_movement_and_zooming();

  game();

  return 0;
}
