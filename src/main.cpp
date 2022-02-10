#include "main.hpp"
#include "tools_runtime.hpp"

void
handleSignal(int signal)
{
    spdlog::shutdown();

    std::exit(signal);
}

int
main()
{
    using namespace std;
    using namespace std::chrono;
    using namespace std::placeholders;

    using namespace spdlog;
    using namespace spdlog::sinks;
    using namespace spdlog::level;

    using namespace nd::src::tools;
    using namespace nd::src::graphics::vulkan;
    using namespace nd::src::graphics::glfw;

    std::signal(SIGABRT, handleSignal);
    std::signal(SIGFPE, handleSignal);
    std::signal(SIGILL, handleSignal);
    std::signal(SIGINT, handleSignal);
    std::signal(SIGSEGV, handleSignal);
    std::signal(SIGTERM, handleSignal);

    const auto maxSize  = 1024 * 1024 * 8;
    const auto maxFiles = 8;

    auto fileSinkMainPtr  = shared<rotating_file_sink_st>(new rotating_file_sink_st("log/log.txt", maxSize, maxFiles));
    auto fileSinkScopePtr = shared<rotating_file_sink_st>(new rotating_file_sink_st("log/scope.txt", maxSize, maxFiles));

    auto logMain  = shared<logger>(new logger(logMainName, {fileSinkMainPtr}));
    auto logScope = shared<logger>(new logger(logScopeName, {fileSinkScopePtr}));

    register_logger(logMain);
    register_logger(logScope);

    spdlog::set_level(level_enum::trace);
    logMain->set_level(level_enum::trace);
    logScope->set_level(level_enum::trace);

    Scope::set(logScope);

    glfwInit();

    const auto window = getWindow({"nd-engine", 800, 600});

    const auto createSurfaceLambda = bind(nd::src::graphics::glfw::createSurface, ref(window.handle), _1, ND_VULKAN_ALLOCATION_CALLBACKS);

    auto vulkanObjects = createVulkanObjects({.applicationName = "nd-application",
                                              .engineName      = "nd-engine",
                                              .layers          = {},
                                              .extensions      = getGlfwRequiredExtensions(),
                                              .width           = window.width,
                                              .height          = window.height},
                                             VulkanObjectsCfgBuilder::getDefault(),
                                             VulkanObjectsInitBuilder::getDefault() << createSurfaceLambda);

    const auto deltaMin = 1.0 / (1 << 16);

    while(!glfwWindowShouldClose(window.handle))
    {
        glfwPollEvents();

        draw(vulkanObjects, getDt(deltaMin));
    }

    destroyVulkanObjects(vulkanObjects);

    glfwTerminate();

    return 0;
}
