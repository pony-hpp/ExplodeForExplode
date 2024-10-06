#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include <string>

namespace gl {
class Shader {
public:
  virtual ~Shader() noexcept;

  virtual int gl_type() const noexcept = 0;

  void load(const char *filename) noexcept;
  void compile() noexcept;
  unsigned gl_handle() const noexcept;

private:
  unsigned _glHandle;
  std::string _src;
};

#define SHADER(className) \
  class className final : public Shader { \
  public: \
    int gl_type() const noexcept override; \
  };

SHADER(VertexShader)
SHADER(FragmentShader)
}

#endif
