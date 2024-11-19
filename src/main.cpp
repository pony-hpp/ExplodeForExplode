#include "core/exception.hpp"
#include "core/logger.hpp"
#include "core/rand.hpp"
#include "game/dynamites/dynamite.hpp"
#include "game/world/generators/plain_world_generator.hpp"
#include "game/world/structures/structure_id.hpp"
#include "opengl/shading/shader_program.hpp"
#include "opengl/shading/uniforms.hpp"
#include "opengl/window/camera.hpp"
#include "opengl/window/renderer.hpp"
#include "opengl/window/window.hpp"

#include <memory>
#include <thread>
#include <vector>

class Game final
{
public:
  Game() noexcept : _logger("Game")
  {
    _logger.info_fmt("Welcome to Explode for Explode %s!", EFE_VERSION);
  }

  ~Game() noexcept { _logger.info_fmt("Goodbye!"); }

  bool setup_win() noexcept
  {
    try
    {
      _win = std::make_unique<gl::Window>(1024, 768, "Explode for Explode");
    }
    catch (const gl::WindowCreationException &e)
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

    return true;
  }

  bool setup_renderer() noexcept
  {
    try
    {
      _renderer = std::make_unique<gl::Renderer>(*_win);
    }
    catch (const gl::RendererInitializationException &e)
    {
      _logger.critical_fmt(
        "Renderer couldn't be initialized: %s.", e.msg.c_str()
      );
      return false;
    }

    _renderer->clear_color(165, 190, 251);

    return true;
  }

  bool setup_shaders() noexcept
  {
    _worldShaderProgram    = std::make_unique<gl::ShaderProgram>("World");
    _dynamiteShaderProgram = std::make_unique<gl::ShaderProgram>("Dynamite");
    _pondShaderProgram     = std::make_unique<gl::ShaderProgram>("Pond");

    try
    {
      { // World shader program
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
      { // Dynamite shader program
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
      { // Pond shader program
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

    game::Dynamite::shader_program(*_dynamiteShaderProgram);

    return true;
  }

  void setup_camera() noexcept
  {
    gl::CameraSettings settings;
    settings.win         = _win.get();
    settings.renderer    = _renderer.get();
    settings.mvButton    = gl::MOUSE_BUTTON_MID;
    settings.sensitivity = 1.4f;
    settings.maxZoomIn   = 5.0f;
    settings.maxZoomOut  = 0.5f;

    settings.initPosX = _worldSettings.w * game::Block::SIZE / 2.0f;
    settings.initPosY = -((_worldSettings.h() + 5) * game::Block::SIZE);

    _cam = std::make_unique<gl::Camera>(settings);
  }

  void create_world_settings() noexcept
  {
    _worldSettings.w      = 200;
    _worldSettings.layers = {
      {game::blocks::GRASS_BLOCK, 1},
      {game::blocks::EARTH_BLOCK, 34},
      {game::blocks::STONE_BLOCK, 65}
    };

    _worldShaderProgram->set_uniform(
      gl::UNIFORM_WORLD_H, _worldSettings.h() * game::Block::SIZE
    );

    core::Rand::init();
    uint maxPondH = 0;
    for (uint i = 0; i < _worldSettings.w; i++)
    {
      if (core::Rand::next(0, 15) == 0)
      {
        _structuresGen.push({game::structures::TREE, (int)i, _worldSettings.h()}
        );
      }
      else if (core::Rand::next(0, 6) != 0)
      {
        _worldSettings.overridenBlocks.push_back(
          {game::blocks::GRASS, (int)i, _worldSettings.h()}
        );
      }
      else if (core::Rand::next(0, 8) == 0)
      {
        _structuresGen.push(
          {game::structures::POND, (int)i, _worldSettings.h() - 1}
        );

        const auto &kPond = _structuresGen.structuresData().back();

        if (i + kPond.w() >= _worldSettings.w)
        {
          _structuresGen.pop();
          continue;
        }

        if (kPond.h() > maxPondH)
        {
          maxPondH = kPond.h();
        }

        i += kPond.w() - 1;
      }
    }

    _worldSettings.structuresGenerator = &_structuresGen;

    _pondShaderProgram->set_uniform(
      gl::UNIFORM_POND_TOP, (_worldSettings.h() - 1) * game::Block::SIZE
    );

    _pondShaderProgram->set_uniform(
      gl::UNIFORM_POND_BOTTOM,
      (_worldSettings.h() - 1 - maxPondH - 10) * game::Block::SIZE
    );
  }

  void generate_world() noexcept
  {
    game::PlainWorldGenerator worldGen(_worldSettings);
    _world = std::make_unique<game::World>(*worldGen());
    _world->load_textures(_pngDecoder);
  }

  void operator()() noexcept
  {
    _win->on_mouse_click(
      [this](gl::MouseButton btn, gl::MouseAction action) noexcept
    {
      if (btn == gl::MOUSE_BUTTON_LEFT && action == gl::MOUSE_ACTION_PRESS)
      {
        _create_dynamite();
      }
    }
    );

    while (!_win->closed())
    {
      _win->poll_events();

      _renderer->clear();

      for (auto &dynamite : _dynamites)
      {
        if (dynamite.first && dynamite.second)
        {
          dynamite.first->explode();
          dynamite.first.reset();
        }
      }

      _renderer->draw(*_world);

      for (const auto &dynamite : _dynamites)
      {
        if (dynamite.first)
        {
          _renderer->draw(*dynamite.first);
        }
      }

      _renderer->update();
    }
  }

private:
  using _tGameDynamite = std::pair<std::unique_ptr<game::Dynamite>, bool>;

  std::unique_ptr<gl::Window> _win;
  std::unique_ptr<gl::Camera> _cam;
  std::unique_ptr<gl::Renderer> _renderer;
  std::unique_ptr<gl::ShaderProgram> _worldShaderProgram,
    _dynamiteShaderProgram, _pondShaderProgram;

  core::PngDecoder _pngDecoder;

  game::PlainWorldGeneratorSettings _worldSettings;
  game::StructuresGenerator _structuresGen;
  std::unique_ptr<game::World> _world;

  uint _explodedDynamites = 0;
  std::vector<_tGameDynamite> _dynamites;

  core::Logger _logger;

  void _create_dynamite() noexcept
  {
    std::thread([this]() noexcept
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(750));

      if (!_win->closed()) // Avoid UB when the window is closed
      {
        _dynamites[_explodedDynamites++].second = true;
      }
    }).detach();

    _dynamites.push_back({std::make_unique<game::Dynamite>(*_world), false});

    _dynamites.back().first->load_texture(_pngDecoder);

    const float kX = _cam->zoom().offsetX / _cam->zoom().scale + _cam->mv().x +
                     _win->cursor_x() / _cam->zoom().scale,
                kY = _cam->zoom().offsetY / _cam->zoom().scale + _cam->mv().y +
                     _win->cursor_y() / _cam->zoom().scale;
    _dynamites.back().first->set_pos(kX, kY);

    _logger.info_fmt("Created dynamite at (%.1f; %.1f).", kX, kY);
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

  if (!game.setup_shaders())
  {
    return 1;
  }

  game.create_world_settings();
  game.generate_world();

  game.setup_camera();

  game();

  return 0;
}
