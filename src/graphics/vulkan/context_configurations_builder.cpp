#include "context_configurations_builder.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    VulkanConfigurationsBuilder configurationsBuilder = VulkanConfigurationsBuilder {} << //
        getInstanceConfiguration <<                                                       //
        getPhysicalDeviceConfiguration <<                                                 //
        getDeviceConfiguration <<                                                         //
        getDeviceMemoryConfigurations <<                                                  //
        getSwapchainConfiguration <<                                                      //
        getRenderPassConfiguration <<                                                     //
        getSwapchainImageViewConfigurations <<                                            //
        getSwapchainFramebufferConfigurations <<                                          //
        getShaderModulesConfigurations <<                                                 //
        getDescriptorPoolConfiguration <<                                                 //
        getDescriptorSetLayoutConfigurations <<                                           //
        getDescriptorSetConfiguration <<                                                  //
        getPipelineLayoutConfigurations <<                                                //
        getGraphicsPipelineConfigurations <<                                              //
        getCommandPoolConfigurations <<                                                   //
        getCommandBufferConfigurations <<                                                 //
        getBufferConfigurations;

    VulkanConfigurationsBuilder::Type
    VulkanConfigurationsBuilder::build() const
    {
        ND_SET_SCOPE();

        ND_ASSERT(getInstance &&              //
                  getPhysicalDevice &&        //
                  getDevice &&                //
                  getDeviceMemories &&        //
                  getSwapchain &&             //
                  getRenderPass &&            //
                  getSwapchainImageViews &&   //
                  getSwapchainFramebuffers && //
                  getShaderModules &&         //
                  getDescriptorPool &&        //
                  getDescriptorSetLayouts &&  //
                  getDescriptorSet &&         //
                  getPipelineLayouts &&       //
                  getGraphicsPipelines &&     //
                  getCommandPools &&          //
                  getCommandBuffers &&        //
                  getBuffers);

        return {getInstance,
                getPhysicalDevice,
                getDevice,
                getDeviceMemories,
                getSwapchain,
                getRenderPass,
                getSwapchainImageViews,
                getSwapchainFramebuffers,
                getShaderModules,
                getDescriptorPool,
                getDescriptorSetLayouts,
                getDescriptorSet,
                getPipelineLayouts,
                getGraphicsPipelines,
                getCommandPools,
                getCommandBuffers,
                getBuffers};
    }
} // namespace nd::src::graphics::vulkan
