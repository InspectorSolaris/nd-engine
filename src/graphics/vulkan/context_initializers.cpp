#include "context_initializers.hpp"
#include "tools.hpp"

namespace nd::src::graphics::vulkan
{
    using namespace nd::src::tools;

    auto initializersBuilder = VulkanContextInitializersBuilder {} << //
        getInstance <<                                                //
        getPhysicalDevice <<                                          //
        getDevice <<                                                  //
        getSwapchain <<                                               //
        getRenderPass <<                                              //
        getSwapchainImages <<                                         //
        getImageViews <<                                              //
        getFramebuffers <<                                            //
        getShaderModules <<                                           //
        getDescriptorPool <<                                          //
        getDescriptorSetLayouts <<                                    //
        getDescriptorSet <<                                           //
        getPipelineLayouts <<                                         //
        getGraphicsPipelines <<                                       //
        getCommandPools <<                                            //
        getCommandBuffers;

    VulkanContextInitializersBuilder::Type
    VulkanContextInitializersBuilder::build() const
    {
        ND_SET_SCOPE();

        ND_ASSERT(getInstance &&              //
                  getPhysicalDevice &&        //
                  getDevice &&                //
                  getSurface &&               //
                  getSwapchain &&             //
                  getRenderPass &&            //
                  getSwapchainImages &&       //
                  getSwapchainImageViews &&   //
                  getSwapchainFramebuffers && //
                  getShaderModules &&         //
                  getDescriptorPool &&        //
                  getDescriptorSetLayouts &&  //
                  getDescriptorSet &&         //
                  getPipelineLayouts &&       //
                  getGraphicsPipelines &&     //
                  getCommandPools &&          //
                  getCommandBuffers);

        return {getInstance,
                getPhysicalDevice,
                getDevice,
                getSurface,
                getSwapchain,
                getRenderPass,
                getSwapchainImages,
                getSwapchainImageViews,
                getSwapchainFramebuffers,
                getShaderModules,
                getDescriptorPool,
                getDescriptorSetLayouts,
                getDescriptorSet,
                getPipelineLayouts,
                getGraphicsPipelines,
                getCommandPools,
                getCommandBuffers};
    }
} // namespace nd::src::graphics::vulkan
