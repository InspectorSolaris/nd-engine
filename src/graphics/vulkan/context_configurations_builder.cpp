#include "context_configurations_builder.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    auto configurationsBuilder = VulkanConfigurationsBuilder {} << //
        getInstanceConfiguration <<                                //
        getPhysicalDeviceConfiguration <<                          //
        getDeviceConfiguration <<                                  //
        getSwapchainConfiguration <<                               //
        getRenderPassConfiguration <<                              //
        getSwapchainImageViewConfigurations <<                     //
        getSwapchainFramebufferConfigurations <<                   //
        getShaderModulesConfigurations <<                          //
        getDescriptorPoolConfiguration <<                          //
        getDescriptorSetLayoutConfigurations <<                    //
        getDescriptorSetConfiguration <<                           //
        getPipelineLayoutConfigurations <<                         //
        getGraphicsPipelineConfigurations <<                       //
        getCommandPoolConfigurations <<                            //
        getCommandBufferConfigurations <<                          //
        getBufferConfigurations <<                                 //
        getAllBufferMemoryConfigurations;

    VulkanConfigurationsBuilder::Type
    VulkanConfigurationsBuilder::build() const
    {
        ND_SET_SCOPE();

        ND_ASSERT(getInstance &&              //
                  getPhysicalDevice &&        //
                  getDevice &&                //
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
                  getBuffers &&               //
                  getBufferMemories);

        return {getInstance,
                getPhysicalDevice,
                getDevice,
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
                getBuffers,
                getBufferMemories};
    }
} // namespace nd::src::graphics::vulkan
