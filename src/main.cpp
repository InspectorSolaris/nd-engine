#include "main.hpp"
#include "tools.hpp"

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

    auto fileSinkMainPtr  = shared_ptr<rotating_file_sink_st>(new rotating_file_sink_st("log/log.txt", maxSize, maxFiles));
    auto fileSinkScopePtr = shared_ptr<rotating_file_sink_st>(new rotating_file_sink_st("log/scope.txt", maxSize, maxFiles));

    auto logMain  = shared_ptr<logger>(new logger(logMainName, {fileSinkMainPtr}));
    auto logScope = shared_ptr<logger>(new logger(logScopeName, {fileSinkScopePtr}));

    register_logger(logMain);
    register_logger(logScope);

    spdlog::set_level(level_enum::trace);
    logMain->set_level(level_enum::trace);
    logScope->set_level(level_enum::trace);

    Scope::set(logScope);

    glfwInit();

    const auto windowConfig = WindowConfiguration {"nd-engine", 800, 600};
    const auto window       = getWindow(windowConfig);

    auto vulkanContext = getVulkanContext({windowConfig.title,
                                           windowConfig.title,
                                           {},
                                           getRequiredExtensions(),
                                           static_cast<uint32_t>(windowConfig.width),
                                           static_cast<uint32_t>(windowConfig.height)},
                                          initializersBuilder << bind(getSurface, cref(window), _1),
                                          configurationsBuilder);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        vulkanContext.drawNextFrame();
    }

    glfwTerminate();

    return 0;
}
