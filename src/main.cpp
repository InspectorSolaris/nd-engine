#include "main.hpp"
#include "tools.hpp"

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

    try
    {
        ND_SET_SCOPE();

        const auto windowConfiguration = nd::src::graphics::glfw::WindowConfiguration {"nd-engine", 800, 600};

        const auto glfwContext = getGlfwContext();
        const auto glfwWindow  = getWindow(windowConfiguration);

        auto vulkanContext = getVulkanContext({windowConfiguration.title,
                                               windowConfiguration.title,
                                               {},
                                               getRequiredExtensions(),
                                               static_cast<uint32_t>(windowConfiguration.width),
                                               static_cast<uint32_t>(windowConfiguration.height)},
                                              {getInstance, bind(getSurface, ref(glfwWindow), _1)},
                                              vulkanContextConfigurations);

        while(!glfwWindowShouldClose(glfwWindow))
        {
            glfwPollEvents();

            vulkanContext.drawNextFrame();
        }
    }
    catch(std::runtime_error error)
    {
        logMain->critical(error.what());
    }

    return 0;
}
