#include "context_initializers_builder.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    VulkanInitializersBuilder initializersBuilder = VulkanInitializersBuilder {} << //
        getInstance <<                                                              //
        getPhysicalDevice <<                                                        //
        getDevice <<                                                                //
        getDeviceMemories <<                                                        //
        getSwapchain <<                                                             //
        getRenderPass <<                                                            //
        getSwapchainImages <<                                                       //
        getImageViews <<                                                            //
        getFramebuffers <<                                                          //
        getShaderModules <<                                                         //
        getDescriptorPool <<                                                        //
        getDescriptorSetLayouts <<                                                  //
        getDescriptorSets <<                                                        //
        getPipelineLayouts <<                                                       //
        getGraphicsPipelines <<                                                     //
        getCommandPools <<                                                          //
        getCommandBuffers <<                                                        //
        getBuffers <<                                                               //
        bindBufferMemory <<                                                         //
        bindImageMemory;

    VulkanInitializersBuilder::Type
    VulkanInitializersBuilder::build() const
    {
        ND_SET_SCOPE();

        ND_ASSERT(getInstance &&              //
                  getPhysicalDevice &&        //
                  getDevice &&                //
                  getDeviceMemories &&        //
                  getSurface &&               //
                  getSwapchain &&             //
                  getRenderPass &&            //
                  getSwapchainImages &&       //
                  getSwapchainImageViews &&   //
                  getSwapchainFramebuffers && //
                  getShaderModules &&         //
                  getDescriptorPool &&        //
                  getDescriptorSetLayouts &&  //
                  getDescriptorSets &&        //
                  getPipelineLayouts &&       //
                  getGraphicsPipelines &&     //
                  getCommandPools &&          //
                  getCommandBuffers &&        //
                  getBuffers &&               //
                  bindBufferMemory &&         //
                  bindImageMemory);

        return {getInstance,
                getPhysicalDevice,
                getDevice,
                getDeviceMemories,
                getSurface,
                getSwapchain,
                getRenderPass,
                getSwapchainImages,
                getSwapchainImageViews,
                getSwapchainFramebuffers,
                getShaderModules,
                getDescriptorPool,
                getDescriptorSetLayouts,
                getDescriptorSets,
                getPipelineLayouts,
                getGraphicsPipelines,
                getCommandPools,
                getCommandBuffers,
                getBuffers,
                bindBufferMemory,
                bindImageMemory};
    }
} // namespace nd::src::graphics::vulkan
