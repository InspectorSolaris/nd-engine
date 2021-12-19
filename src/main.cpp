#include "main.hpp"
#include "tools.hpp"

int
main()
{
    using namespace spdlog;
    using namespace spdlog::sinks;

    using namespace std::placeholders;
    using namespace nd::src::tools;
    using namespace nd::src::graphics;

    const auto maxSize  = 1024 * 1024 * 8;
    const auto maxFiles = 8;

    auto fileSinkMainPtr =
        std::shared_ptr<rotating_file_sink_st>(new rotating_file_sink_st("log/log.txt", maxSize, maxFiles));

    auto fileSinkScopePtr =
        std::shared_ptr<rotating_file_sink_st>(new rotating_file_sink_st("log/scope.txt", maxSize, maxFiles));

    auto logMain  = std::shared_ptr<logger>(new logger(logMainName, {fileSinkMainPtr}));
    auto logScope = std::shared_ptr<logger>(new logger(logScopeName, {fileSinkScopePtr}));

    register_logger(logMain);
    register_logger(logScope);

    spdlog::set_level(level::level_enum::trace);
    logMain->set_level(level::level_enum::trace);
    logScope->set_level(level::level_enum::trace);

    Scope::set(logScope);

    try
    {
        ND_SET_SCOPE();

        const auto name = std::string("nd-engine");

        auto glfwContext = glfw::getContext();
        auto glfwWindow  = glfw::getWindow({name, 800, 600});

        auto vulkanContext = vulkan::getContext({std::bind(glfw::getSurface, std::ref(glfwWindow), _1),
                                                 name,
                                                 name,
                                                 {},
                                                 glfw::getRequiredExtensions(),
                                                 static_cast<uint32_t>(glfwWindow.getWidth()),
                                                 static_cast<uint32_t>(glfwWindow.getHeight())});

        while(!glfwWindowShouldClose(glfwWindow.get()))
        {
            glfwPollEvents();
        }
    }
    catch(std::runtime_error error)
    {
        logMain->critical(error.what());
    }

    return 0;
}
