#include "main.hpp"
#include "tools.hpp"

int
main()
{
    using namespace nd::src::tools;
    using namespace nd::src::graphics;

    using Log            = spdlog::logger;
    using FileSink       = spdlog::sinks::rotating_file_sink_st;
    using ConsoleSink    = spdlog::sinks::stdout_sink_st;
    using LogPtr         = std::shared_ptr<Log>;
    using FileSinkPtr    = std::shared_ptr<FileSink>;
    using ConsoleSinkPtr = std::shared_ptr<ConsoleSink>;

    const auto maxSize  = 1024 * 1024 * 8;
    const auto maxFiles = 8;

    auto fileSinkScopePtr    = FileSinkPtr(new FileSink("log/scope.txt", maxSize, maxFiles));
    auto consoleSinkScopePtr = ConsoleSinkPtr(new ConsoleSink());
    auto logScope            = Scope::LogPtr(new Scope::Log(logScopeName, {fileSinkScopePtr, consoleSinkScopePtr}));

    auto fileSinkMainPtr    = FileSinkPtr(new FileSink("log/log.txt", maxSize, maxFiles));
    auto consoleSinkMainPtr = ConsoleSinkPtr(new ConsoleSink());
    auto logMain            = LogPtr(new Log(logMainName, {fileSinkMainPtr, consoleSinkMainPtr}));

    logScope->set_level(spdlog::level::level_enum::trace);
    logMain->set_level(spdlog::level::level_enum::trace);

    Scope::set(logScope);

    spdlog::register_logger(logMain);

    ND_SET_SCOPE();

    auto  glfwContext = glfw::Context {};
    auto& glfwWindow  = glfwContext.getWindow();

    const auto getSurface = [&glfwWindow](const VkInstance instance)
    {
        return glfwWindow.getSurface(instance);
    };

    auto vulkanContext = vulkan::Context {
        {getSurface,
         {},
         glfw::getRequiredExtensions(),
         static_cast<uint32_t>(glfwWindow.getWidth()),
         static_cast<uint32_t>(glfwWindow.getHeight())}
    };

    while(!glfwWindowShouldClose(glfwWindow.get()))
    {
        glfwPollEvents();
    }

    return 0;
}
