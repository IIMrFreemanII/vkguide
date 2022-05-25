#include "log.h"
#include "vk_engine.h"

int main() {
  log::init();
  log::info("Hello world");

  VulkanEngine engine;

  engine.init();

  engine.run();

  engine.cleanup();

  return 0;
}
