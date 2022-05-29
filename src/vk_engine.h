#pragma once

#include <vector>
#include <deque>
#include <functional>
#include <unordered_map>

#include <glm/glm.hpp>
#include <VkBootstrap.h>
#include <vk_mem_alloc.h>

#include "vk_types.h"
#include "vk_mesh.h"

struct Texture {
  AllocatedImage image;
  VkImageView imageView;
};

struct UploadContext {
  VkFence _uploadFence;
  VkCommandPool _commandPool;
  VkCommandBuffer _commandBuffer;
};

struct GPUObjectData{
  glm::mat4 modelMatrix;
};

struct GPUSceneData {
  glm::vec4 fogColor; // w is for exponent
  glm::vec4 fogDistances; //x for min, y for max, zw unused.
  glm::vec4 ambientColor;
  glm::vec4 sunlightDirection; //w for sun power
  glm::vec4 sunlightColor;
};

struct GPUCameraData{
  glm::mat4 view;
  glm::mat4 proj;
  glm::mat4 viewproj;
};

struct FrameData {
  VkSemaphore _presentSemaphore, _renderSemaphore;
  VkFence _renderFence;

  VkCommandPool _commandPool;
  VkCommandBuffer _mainCommandBuffer;

  //buffer that holds a single GPUCameraData to use when rendering
  AllocatedBuffer cameraBuffer;
  VkDescriptorSet globalDescriptor;

  AllocatedBuffer objectBuffer;
  VkDescriptorSet objectDescriptor;
};

struct Material {
  VkDescriptorSet textureSet{VK_NULL_HANDLE};
  VkPipeline pipeline;
  VkPipelineLayout pipelineLayout;
};

struct RenderObject {
  Mesh* mesh;

  Material* material;

  glm::mat4 transformMatrix;
};

struct MeshPushConstants {
  glm::vec4 data;
  // model matrix
  glm::mat4 render_matrix;
};

struct DeletionQueue
{
  std::deque<std::function<void()>> deletors;

  void push_function(std::function<void()>&& function) {
    deletors.push_front(function);
  }

  void flush() {
    for (const auto& func : deletors) {
      func();
    }

    deletors.clear();
  }
};

class PipelineBuilder {
public:

  std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
  VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
  VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
  VkViewport _viewport;
  VkRect2D _scissor;
  VkPipelineRasterizationStateCreateInfo _rasterizer;
  VkPipelineColorBlendAttachmentState _colorBlendAttachment;
  VkPipelineMultisampleStateCreateInfo _multisampling;
  VkPipelineLayout _pipelineLayout;
  VkPipelineDepthStencilStateCreateInfo _depthStencil;

  VkPipeline build_pipeline(VkDevice device, VkRenderPass pass);
};

//number of frames to overlap when rendering
constexpr uint32_t FRAME_OVERLAP = 2;

class VulkanEngine {
public:
  //texture hashmap
  std::unordered_map<std::string, Texture> _loadedTextures;
  void load_images();

  UploadContext _uploadContext;
  void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);

  GPUSceneData _sceneParameters;
  AllocatedBuffer _sceneParameterBuffer;

  VkPhysicalDeviceProperties _gpuProperties;

  VkDescriptorSetLayout _globalSetLayout;
  VkDescriptorSetLayout _objectSetLayout;
  VkDescriptorSetLayout _singleTextureSetLayout;
  VkDescriptorPool _descriptorPool;

  VmaAllocator _allocator;
  DeletionQueue _mainDeletionQueue;

  bool _isInitialized{false};
  uint32_t _frameNumber{0};

  int _selectedShader{ 0 };

  VkExtent2D _windowExtent{
    800,
    600
  };

  struct SDL_Window* _window{nullptr};

  VkInstance _instance; // Vulkan library handle
  VkDebugUtilsMessengerEXT _debug_messenger; // Vulkan debug output handle
  VkPhysicalDevice _chosenGPU; // GPU chosen as the default device
  vkb::Device vkbDevice;
  VkDevice _device; // Vulkan device for commands
  VkSurfaceKHR _surface; // Vulkan window surface

  VkSwapchainKHR _swapchain; // from other articles
  // image format expected by the windowing system
  VkFormat _swapchainImageFormat;
  //array of images from the swapchain
  std::vector<VkImage> _swapchainImages;
  //array of image-views from the swapchain
  std::vector<VkImageView> _swapchainImageViews;

  VkImageView _depthImageView;
  AllocatedImage _depthImage;
  //the format for the depth image
  VkFormat _depthFormat;

  VkQueue _graphicsQueue; //queue we will submit to
  uint32_t _graphicsQueueFamily; //family of that queue

  //frame storage
  FrameData _frames[FRAME_OVERLAP];

  //getter for the frame we are rendering to right now.
  FrameData& get_current_frame();

  VkRenderPass _renderPass;
  std::vector<VkFramebuffer> _framebuffers;

public:
  //default array of renderable objects
  std::vector<RenderObject> _renderables;

  std::unordered_map<std::string, Material> _materials;
  std::unordered_map<std::string, Mesh> _meshes;
  //functions

  //create material and add it to the map
  Material* create_material(VkPipeline pipeline, VkPipelineLayout layout, const std::string& name);

  //returns nullptr if it can't be found
  Material* get_material(const std::string& name);

  //returns nullptr if it can't be found
  Mesh* get_mesh(const std::string& name);

  //our draw function
  void draw_objects(VkCommandBuffer cmd, RenderObject* first, int count);

public:
  //initializes everything in the engine
  void init();

  //shuts down the engine
  void cleanup();

  //draw loop
  void draw();

  //run main loop
  void run();

private:

  void init_vulkan();
  void init_swapchain();
  void init_commands();

  void init_default_renderpass();
  void init_framebuffers();

  void init_sync_structures();

  void init_pipelines();

  void init_scene();

  //loads a shader module from a spir-v file. Returns false if it errors
  bool load_shader_module(const char* filePath, VkShaderModule* outShaderModule);

  void load_meshes();

  void upload_mesh(Mesh& mesh);

  void init_descriptors();

public:

  AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

  size_t pad_uniform_buffer_size(size_t originalSize);
};